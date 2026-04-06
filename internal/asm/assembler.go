package asm

import (
	"fmt"
	"strconv"
	"strings"
)

// Result holds the output of a successful assembly.
type Result struct {
	Binary   []byte
	Warnings []string
}

// Error is returned when assembly fails.
type Error struct {
	Line    int
	Message string
}

func (e *Error) Error() string {
	return fmt.Sprintf("line %d: %s", e.Line, e.Message)
}

// ── Opcode tables ─────────────────────────────────────────────────────────────

// impliedOps maps mnemonics with no operands to their opcodes.
var impliedOps = map[string]byte{
	"NOP": 0x00, "HCF": 0x01, "PUSH": 0x02, "POP": 0x03, "RET": 0x08,
	"CSF": 0xA0, "CZF": 0xA1, "SZF": 0xA2, "CNF": 0xA3, "SNF": 0xA4,
	"CCF": 0xA5, "SCF": 0xA6, "COF": 0xA7, "SOF": 0xA8, "CLF": 0xA9,
	"SLF": 0xAA, "CIF": 0xAB, "SIF": 0xAC, "CEF": 0xAD,
}

// opcodeMap maps "MODE MNEMONIC" or "SRCMODE DSTMODE MNEMONIC" → opcode byte.
var opcodeMap = map[string]byte{
	// CALL
	"IMM CALL": 0x05, "ABS CALL": 0x06, "IND CALL": 0x07,
	// ADD / ADDX
	"IMM ADD": 0x10, "ABS ADD": 0x11, "IND ADD": 0x12, "REG ADD": 0x13,
	"IMM ADDX": 0x14, "ABS ADDX": 0x15, "IND ADDX": 0x16, "REG ADDX": 0x17,
	// SUB / SUBX
	"IMM SUB": 0x18, "ABS SUB": 0x19, "IND SUB": 0x1A, "REG SUB": 0x1B,
	"IMM SUBX": 0x1C, "ABS SUBX": 0x1D, "IND SUBX": 0x1E, "REG SUBX": 0x1F,
	// MUL / MULX
	"IMM MUL": 0x20, "ABS MUL": 0x21, "IND MUL": 0x22, "REG MUL": 0x23,
	"IMM MULX": 0x24, "ABS MULX": 0x25, "IND MULX": 0x26, "REG MULX": 0x27,
	// DIV / DIVX
	"IMM DIV": 0x28, "ABS DIV": 0x29, "IND DIV": 0x2A, "REG DIV": 0x2B,
	"IMM DIVX": 0x2C, "ABS DIVX": 0x2D, "IND DIVX": 0x2E, "REG DIVX": 0x2F,
	// MOV (src dst)
	"IMM ABS MOV": 0x30, "ABS ABS MOV": 0x31, "IND ABS MOV": 0x32, "REG ABS MOV": 0x33,
	"IMM IND MOV": 0x34, "ABS IND MOV": 0x35, "IND IND MOV": 0x36, "REG IND MOV": 0x37,
	"IMM REG MOV": 0x38, "ABS REG MOV": 0x39, "IND REG MOV": 0x3A, "REG REG MOV": 0x3B,
	// SHL / SHLX
	"IMM SHL": 0x3C, "ABS SHL": 0x3D, "IND SHL": 0x3E, "REG SHL": 0x3F,
	// MOVX (src dst)
	"IMM ABS MOVX": 0x40, "ABS ABS MOVX": 0x41, "IND ABS MOVX": 0x42, "REG ABS MOVX": 0x43,
	"IMM IND MOVX": 0x44, "ABS IND MOVX": 0x45, "IND IND MOVX": 0x46, "REG IND MOVX": 0x47,
	"IMM REG MOVX": 0x48, "ABS REG MOVX": 0x49, "IND REG MOVX": 0x4A, "REG REG MOVX": 0x4B,
	"IMM SHLX": 0x4C, "ABS SHLX": 0x4D, "IND SHLX": 0x4E, "REG SHLX": 0x4F,
	// AND / OR / XOR
	"IMM AND": 0x50, "ABS AND": 0x51, "IND AND": 0x52, "REG AND": 0x53,
	"IMM OR": 0x54, "ABS OR": 0x55, "IND OR": 0x56, "REG OR": 0x57,
	"IMM XOR": 0x58, "ABS XOR": 0x59, "IND XOR": 0x5A, "REG XOR": 0x5B,
	// SHR / SHRX
	"IMM SHR": 0x5C, "ABS SHR": 0x5D, "IND SHR": 0x5E, "REG SHR": 0x5F,
	// CMP / CMPX
	"IMM CMP": 0x60, "ABS CMP": 0x61, "IND CMP": 0x62, "REG CMP": 0x63,
	"IMM CMPX": 0x64, "ABS CMPX": 0x65, "IND CMPX": 0x66, "REG CMPX": 0x67,
	"IMM SHRX": 0x6C, "ABS SHRX": 0x6D, "IND SHRX": 0x6E, "REG SHRX": 0x6F,
	// JMP and conditionals
	"IMM JMP": 0x70, "ABS JMP": 0x71, "IND JMP": 0x72, "REG JMP": 0x73,
	"IMM JZS": 0x74, "ABS JZS": 0x75, "IND JZS": 0x76, "REG JZS": 0x77,
	"IMM JNZ": 0x78, "ABS JNZ": 0x79, "IND JNZ": 0x7A, "REG JNZ": 0x7B,
	"IMM JCS": 0x7C, "ABS JCS": 0x7D, "IND JCS": 0x7E, "REG JCS": 0x7F,
	"IMM JNC": 0x80, "ABS JNC": 0x81, "IND JNC": 0x82, "REG JNC": 0x83,
	"IMM JOS": 0x84, "ABS JOS": 0x85, "IND JOS": 0x86, "REG JOS": 0x87,
	"IMM JNO": 0x88, "ABS JNO": 0x89, "IND JNO": 0x8A, "REG JNO": 0x8B,
	"IMM JNS": 0x8C, "ABS JNS": 0x8D, "IND JNS": 0x8E, "REG JNS": 0x8F,
	"IMM JNN": 0x90, "ABS JNN": 0x91, "IND JNN": 0x92, "REG JNN": 0x93,
	"IMM JLS": 0x94, "ABS JLS": 0x95, "IND JLS": 0x96, "REG JLS": 0x97,
	"IMM JNL": 0x98, "ABS JNL": 0x99, "IND JNL": 0x9A, "REG JNL": 0x9B,
	"IMM JES": 0x9C, "ABS JES": 0x9D, "IND JES": 0x9E, "REG JES": 0x9F,
	// INC / INCX / DEC / DECX (no immediate mode)
	"ABS INC": 0xB0, "IND INC": 0xB1, "REG INC": 0xB2,
	"ABS INCX": 0xB3, "IND INCX": 0xB4, "REG INCX": 0xB5,
	"ABS DEC": 0xB6, "IND DEC": 0xB7, "REG DEC": 0xB8,
	"ABS DECX": 0xB9, "IND DECX": 0xBA, "REG DECX": 0xBB,
}

// wide16 lists mnemonics whose IMM operand is 16-bit (2 bytes).
var wide16 = map[string]bool{
	"ADDX": true, "SUBX": true, "MULX": true, "DIVX": true,
	"SHLX": true, "SHRX": true, "CMPX": true, "INCX": true, "DECX": true,
	"JMP": true, "JZS": true, "JNZ": true, "JCS": true, "JNC": true,
	"JOS": true, "JNO": true, "JNS": true, "JNN": true,
	"JLS": true, "JNL": true, "JES": true, "CALL": true,
}

// ── Operand parsing ───────────────────────────────────────────────────────────

// modeOf returns the addressing-mode string for a token without resolving its
// numeric value. Safe to call in pass 1 before labels are known.
func modeOf(token string) string {
	if len(token) == 0 {
		return "ABS"
	}
	switch token[0] {
	case '#':
		return "IMM"
	case '@':
		return "IND"
	}
	// Rn (n = one or more decimal digits, 0–7) → register
	if (token[0] == 'R' || token[0] == 'r') && len(token) >= 2 {
		onlyDigits := true
		for _, ch := range token[1:] {
			if ch < '0' || ch > '9' {
				onlyDigits = false
				break
			}
		}
		if onlyDigits {
			return "REG"
		}
	}
	return "ABS"
}

// resolveOperand parses a token into (mode, value), resolving labels.
// The prefix character (#, @) is stripped before value parsing.
func resolveOperand(token string, labels map[string]uint16) (mode string, value uint16, err error) {
	mode = modeOf(token)
	raw := token

	switch mode {
	case "IMM", "IND":
		raw = token[1:]
	case "REG":
		idx, parseErr := strconv.ParseUint(token[1:], 10, 8)
		if parseErr != nil || idx > 7 {
			return "", 0, fmt.Errorf("invalid register %q", token)
		}
		return "REG", uint16(idx), nil
	}

	// Try numeric (hex)
	v, parseErr := strconv.ParseUint(raw, 16, 16)
	if parseErr == nil {
		return mode, uint16(v), nil
	}

	// Try label
	if addr, ok := labels[strings.ToLower(raw)]; ok {
		return mode, addr, nil
	}

	return "", 0, fmt.Errorf("undefined symbol %q", raw)
}

// ── Instruction sizing (pass 1) ───────────────────────────────────────────────

// operandSize returns the byte count for a serialised operand.
func operandSize(mode, mnem string) int {
	switch mode {
	case "REG":
		return 1
	case "IMM":
		if wide16[mnem] {
			return 2
		}
		return 1
	default: // ABS, IND
		return 2
	}
}

// instrSize returns the total byte count of the instruction at tokens[0..].
// Called in pass 1 where label values are not yet known.
func instrSize(tokens []string) (int, error) {
	mnem := strings.ToUpper(tokens[0])

	if _, ok := impliedOps[mnem]; ok {
		return 1, nil
	}
	if mnem == "SWP" {
		return 3, nil // opcode + reg + reg
	}
	if mnem == "MOV" || mnem == "MOVX" {
		if len(tokens) < 3 {
			return 0, fmt.Errorf("%s requires 2 operands", mnem)
		}
		srcMode := modeOf(tokens[1])
		dstMode := modeOf(tokens[2])
		var srcSz int
		if mnem == "MOVX" {
			if srcMode == "IMM" {
				srcSz = 2
			} else if srcMode == "REG" {
				srcSz = 1
			} else {
				srcSz = 2
			}
		} else {
			if srcMode == "REG" || srcMode == "IMM" {
				srcSz = 1
			} else {
				srcSz = 2
			}
		}
		dstSz := 1
		if dstMode != "REG" {
			dstSz = 2
		}
		return 1 + srcSz + dstSz, nil
	}

	if len(tokens) < 2 {
		return 0, fmt.Errorf("%s requires an operand", mnem)
	}
	return 1 + operandSize(modeOf(tokens[1]), mnem), nil
}

// ── Instruction emission (pass 2) ────────────────────────────────────────────

func emit16(out *[]byte, v uint16) {
	*out = append(*out, byte(v>>8), byte(v))
}

func emitInstruction(out *[]byte, lineNum int, tokens []string, labels map[string]uint16) error {
	mnem := strings.ToUpper(tokens[0])

	// Implied
	if op, ok := impliedOps[mnem]; ok {
		*out = append(*out, op)
		return nil
	}

	// SWP Rx Ry
	if mnem == "SWP" {
		if len(tokens) < 3 {
			return &Error{lineNum, "SWP requires two register operands"}
		}
		m1, v1, err := resolveOperand(tokens[1], labels)
		if err != nil || m1 != "REG" {
			return &Error{lineNum, fmt.Sprintf("SWP: expected register, got %q", tokens[1])}
		}
		m2, v2, err := resolveOperand(tokens[2], labels)
		if err != nil || m2 != "REG" {
			return &Error{lineNum, fmt.Sprintf("SWP: expected register, got %q", tokens[2])}
		}
		*out = append(*out, 0x04, byte(v1), byte(v2))
		return nil
	}

	// MOV / MOVX
	if mnem == "MOV" || mnem == "MOVX" {
		if len(tokens) < 3 {
			return &Error{lineNum, fmt.Sprintf("%s requires 2 operands", mnem)}
		}
		srcMode, srcVal, err := resolveOperand(tokens[1], labels)
		if err != nil {
			return &Error{lineNum, fmt.Sprintf("%s src: %v", mnem, err)}
		}
		dstMode, dstVal, err := resolveOperand(tokens[2], labels)
		if err != nil {
			return &Error{lineNum, fmt.Sprintf("%s dst: %v", mnem, err)}
		}
		key := srcMode + " " + dstMode + " " + mnem
		op, ok := opcodeMap[key]
		if !ok {
			return &Error{lineNum, fmt.Sprintf("invalid addressing for %s: %s → %s", mnem, srcMode, dstMode)}
		}
		*out = append(*out, op)
		// emit source
		if mnem == "MOVX" {
			if srcMode == "REG" {
				*out = append(*out, byte(srcVal))
			} else {
				emit16(out, srcVal)
			}
		} else {
			if srcMode == "REG" || srcMode == "IMM" {
				*out = append(*out, byte(srcVal))
			} else {
				emit16(out, srcVal)
			}
		}
		// emit destination
		if dstMode == "REG" {
			*out = append(*out, byte(dstVal))
		} else {
			emit16(out, dstVal)
		}
		return nil
	}

	// Single-operand instructions
	if len(tokens) < 2 {
		return &Error{lineNum, fmt.Sprintf("%s requires an operand", mnem)}
	}
	opMode, opVal, err := resolveOperand(tokens[1], labels)
	if err != nil {
		return &Error{lineNum, err.Error()}
	}
	key := opMode + " " + mnem
	op, ok := opcodeMap[key]
	if !ok {
		return &Error{lineNum, fmt.Sprintf("unknown instruction: %s %s", mnem, opMode)}
	}
	*out = append(*out, op)
	switch opMode {
	case "IMM":
		if wide16[mnem] {
			emit16(out, opVal)
		} else {
			*out = append(*out, byte(opVal))
		}
	case "ABS", "IND":
		emit16(out, opVal)
	case "REG":
		*out = append(*out, byte(opVal))
	}
	return nil
}

// ── Public API ────────────────────────────────────────────────────────────────

// Assemble takes Cosmic assembly source and returns machine code binary.
// Syntax:
//   - Comments start with ";"
//   - Labels are identifiers ending with ":" (e.g. "loop:")
//   - Numbers are hexadecimal (no 0x prefix)
//   - Addressing modes: #val (immediate), @addr (indirect), Rn (register 0–7), addr (absolute)
func Assemble(source string) (*Result, error) {
	lines := strings.Split(source, "\n")
	labels := make(map[string]uint16)

	// ── Pass 1: collect label addresses ──────────────────────────────────────
	var pc uint16
	for lineNum, line := range lines {
		if idx := strings.IndexByte(line, ';'); idx >= 0 {
			line = line[:idx]
		}
		tokens := strings.Fields(line)
		if len(tokens) == 0 {
			continue
		}
		if strings.HasSuffix(tokens[0], ":") {
			label := strings.ToLower(tokens[0][:len(tokens[0])-1])
			labels[label] = pc
			tokens = tokens[1:]
			if len(tokens) == 0 {
				continue
			}
		}
		size, err := instrSize(tokens)
		if err != nil {
			return nil, &Error{lineNum + 1, err.Error()}
		}
		pc += uint16(size)
	}

	// ── Pass 2: emit bytes ────────────────────────────────────────────────────
	var out []byte
	for lineNum, line := range lines {
		if idx := strings.IndexByte(line, ';'); idx >= 0 {
			line = line[:idx]
		}
		tokens := strings.Fields(line)
		if len(tokens) == 0 {
			continue
		}
		if strings.HasSuffix(tokens[0], ":") {
			tokens = tokens[1:]
			if len(tokens) == 0 {
				continue
			}
		}
		if err := emitInstruction(&out, lineNum+1, tokens, labels); err != nil {
			return nil, err
		}
	}

	return &Result{Binary: out}, nil
}
