package cpu_test

import (
	"encoding/json"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"testing"

	"github.com/clbx/cosmic/internal/asm"
	"github.com/clbx/cosmic/internal/cpu"
)

const defaultMaxCycles = 1_000_000

// ── In-process test helpers ───────────────────────────────────────────────────

// Check describes expected CPU state after execution completes (CPU halted).
// Zero values mean "don't check": nil pointers are skipped, absent map keys are skipped.
type Check struct {
	Registers map[string]uint8  // "A"–"H" → expected byte
	PC        *uint16           // nil = skip
	SP        *uint16           // nil = skip
	Flags     map[string]bool   // "Z","N","C","O","L","I","E" → expected set/clear
	Memory    map[uint16]uint8  // address → expected byte
}

// Case is a single harness test: load binary, run to halt, check state.
type Case struct {
	Name      string
	Binary    []byte // inline machine code; mutually exclusive with File
	File      string // path relative to testdata/ for a .bin
	Origin    uint16 // load address (default 0x0000)
	MaxCycles int    // 0 = defaultMaxCycles
	Expect    Check
}

var regNames = map[string]int{
	"A": 0, "B": 1, "C": 2, "D": 3,
	"E": 4, "F": 5, "G": 6, "H": 7,
}

// ptr16 is a helper for declaring expected 16-bit values inline.
func ptr16(v uint16) *uint16 { return &v }

// runCase executes a single Case under t.
func runCase(t *testing.T, tc Case) {
	t.Helper()
	t.Run(tc.Name, func(t *testing.T) {
		t.Helper()
		executeAndCheck(t, tc.Binary, tc.File, tc.Origin, tc.MaxCycles, tc.Expect)
	})
}

// ── File-driven test discovery ────────────────────────────────────────────────

// testSpec is the JSON schema for a file-driven test case.
//
// All hex values are bare hex strings without "0x" prefix (e.g. "42", "C399").
//
//   {
//     "name":      "descriptive name",
//     "file":      "program.bin",      // load testdata/program.bin; or use "hex" instead
//     "hex":       "38 42 00 01",      // space-separated byte values in hex
//     "origin":    "0000",             // optional load address, default "0000"
//     "maxCycles": 100000,             // optional
//     "expect": {
//       "registers": { "A": "42" },
//       "pc":         "0006",
//       "sp":         "C399",
//       "flags":      { "Z": false, "C": true },
//       "memory":     { "0200": "7F" }
//     }
//   }
type testSpec struct {
	Name      string     `json:"name"`
	File      string     `json:"file"`      // load testdata/<file>.bin
	Hex       string     `json:"hex"`       // space-separated hex bytes
	Asm       string     `json:"asm"`       // inline assembly source
	AsmFile   string     `json:"asmFile"`   // load and assemble testdata/<asmFile>.asm
	Origin    string     `json:"origin"`
	MaxCycles int        `json:"maxCycles"`
	Expect    expectSpec `json:"expect"`
}

type expectSpec struct {
	Registers map[string]string `json:"registers"` // "A" → "42"
	PC        string            `json:"pc"`         // "0006" or ""
	SP        string            `json:"sp"`         // "C399" or ""
	Flags     map[string]bool   `json:"flags"`
	Memory    map[string]string `json:"memory"`     // "0200" → "7F"
}

// TestFromFiles discovers all *.json files in testdata/, parses them as testSpec,
// and runs each as a sub-test. Add a .json (and optional .bin) to testdata/ to
// define a new test without touching Go code.
func TestFromFiles(t *testing.T) {
	paths, err := filepath.Glob("testdata/*.json")
	if err != nil {
		t.Fatalf("glob testdata/*.json: %v", err)
	}
	if len(paths) == 0 {
		t.Skip("no testdata/*.json files found")
	}

	for _, path := range paths {
		path := path
		raw, err := os.ReadFile(path)
		if err != nil {
			t.Errorf("read %s: %v", path, err)
			continue
		}

		// Support both a single spec object and an array of spec objects.
		var specs []testSpec
		if raw[0] == '[' {
			if err := json.Unmarshal(raw, &specs); err != nil {
				t.Errorf("parse %s: %v", path, err)
				continue
			}
		} else {
			var single testSpec
			if err := json.Unmarshal(raw, &single); err != nil {
				t.Errorf("parse %s: %v", path, err)
				continue
			}
			specs = []testSpec{single}
		}

		for _, spec := range specs {
			spec := spec
			name := spec.Name
			if name == "" {
				name = filepath.Base(path)
			}
			t.Run(name, func(t *testing.T) {
				t.Helper()

				// Resolve binary.
				var bin []byte
				switch {
				case spec.File != "":
					bin, err = os.ReadFile("testdata/" + spec.File)
					if err != nil {
						t.Fatalf("load %q: %v", spec.File, err)
					}
				case spec.Hex != "":
					bin, err = parseHexBytes(spec.Hex)
					if err != nil {
						t.Fatalf("parse hex: %v", err)
					}
				case spec.Asm != "":
					result, aErr := asm.Assemble(spec.Asm)
					if aErr != nil {
						t.Fatalf("assemble inline asm: %v", aErr)
					}
					bin = result.Binary
				case spec.AsmFile != "":
					src, readErr := os.ReadFile("testdata/" + spec.AsmFile)
					if readErr != nil {
						t.Fatalf("load %q: %v", spec.AsmFile, readErr)
					}
					result, aErr := asm.Assemble(string(src))
					if aErr != nil {
						t.Fatalf("assemble %q: %v", spec.AsmFile, aErr)
					}
					bin = result.Binary
				default:
					t.Fatal("spec must have one of: \"file\", \"hex\", \"asm\", \"asmFile\"")
				}

				// Parse origin.
				var origin uint16
				if spec.Origin != "" {
					v, err := strconv.ParseUint(spec.Origin, 16, 16)
					if err != nil {
						t.Fatalf("parse origin %q: %v", spec.Origin, err)
					}
					origin = uint16(v)
				}

				// Convert expectSpec → Check.
				check, err := specToCheck(spec.Expect)
				if err != nil {
					t.Fatalf("parse expect: %v", err)
				}

				executeAndCheck(t, bin, "", origin, spec.MaxCycles, check)
			})
		}
	}
}

// ── Shared execution + assertion logic ───────────────────────────────────────

func executeAndCheck(t *testing.T, bin []byte, file string, origin uint16, maxCycles int, expect Check) {
	t.Helper()

	if file != "" {
		data, err := os.ReadFile("testdata/" + file)
		if err != nil {
			t.Fatalf("load %q: %v", file, err)
		}
		bin = data
	}
	if len(bin) == 0 {
		t.Fatal("binary is empty")
	}

	c := cpu.New()
	c.LoadROM(bin, origin)

	limit := maxCycles
	if limit == 0 {
		limit = defaultMaxCycles
	}
	for i := 0; i < limit; i++ {
		if !c.Cycle() {
			break
		}
		if i == limit-1 {
			t.Fatalf("CPU did not halt within %d cycles", limit)
		}
	}

	snap := c.Snapshot()

	for name, want := range expect.Registers {
		idx, ok := regNames[name]
		if !ok {
			t.Errorf("unknown register %q", name)
			continue
		}
		if got := snap.Registers[idx]; got != want {
			t.Errorf("register %s: got 0x%02X, want 0x%02X", name, got, want)
		}
	}
	if expect.PC != nil && snap.PC != *expect.PC {
		t.Errorf("PC: got 0x%04X, want 0x%04X", snap.PC, *expect.PC)
	}
	if expect.SP != nil && snap.SP != *expect.SP {
		t.Errorf("SP: got 0x%04X, want 0x%04X", snap.SP, *expect.SP)
	}

	flagVals := map[string]bool{
		"Z": snap.Flags.Zero, "N": snap.Flags.Negative,
		"C": snap.Flags.Carry, "O": snap.Flags.Overflow,
		"L": snap.Flags.Less, "I": snap.Flags.Interrupt, "E": snap.Flags.Error,
	}
	for name, want := range expect.Flags {
		got, ok := flagVals[name]
		if !ok {
			t.Errorf("unknown flag %q", name)
			continue
		}
		if got != want {
			t.Errorf("flag %s: got %v, want %v", name, got, want)
		}
	}

	for addr, want := range expect.Memory {
		if got := c.Read(addr); got != want {
			t.Errorf("memory[0x%04X]: got 0x%02X, want 0x%02X", addr, got, want)
		}
	}
}

// ── Parsing helpers ───────────────────────────────────────────────────────────

// parseHexBytes parses a space-separated hex byte string like "38 42 00 01".
func parseHexBytes(s string) ([]byte, error) {
	fields := strings.Fields(s)
	out := make([]byte, len(fields))
	for i, f := range fields {
		v, err := strconv.ParseUint(f, 16, 8)
		if err != nil {
			return nil, err
		}
		out[i] = byte(v)
	}
	return out, nil
}

// specToCheck converts the JSON expectSpec into the internal Check type.
func specToCheck(s expectSpec) (Check, error) {
	ch := Check{
		Flags: s.Flags,
	}

	if len(s.Registers) > 0 {
		ch.Registers = make(map[string]uint8, len(s.Registers))
		for k, v := range s.Registers {
			n, err := strconv.ParseUint(v, 16, 8)
			if err != nil {
				return ch, err
			}
			ch.Registers[k] = uint8(n)
		}
	}

	if s.PC != "" {
		v, err := strconv.ParseUint(s.PC, 16, 16)
		if err != nil {
			return ch, err
		}
		u := uint16(v)
		ch.PC = &u
	}
	if s.SP != "" {
		v, err := strconv.ParseUint(s.SP, 16, 16)
		if err != nil {
			return ch, err
		}
		u := uint16(v)
		ch.SP = &u
	}

	if len(s.Memory) > 0 {
		ch.Memory = make(map[uint16]uint8, len(s.Memory))
		for addrStr, valStr := range s.Memory {
			addr, err := strconv.ParseUint(addrStr, 16, 16)
			if err != nil {
				return ch, err
			}
			val, err := strconv.ParseUint(valStr, 16, 8)
			if err != nil {
				return ch, err
			}
			ch.Memory[uint16(addr)] = uint8(val)
		}
	}

	return ch, nil
}
