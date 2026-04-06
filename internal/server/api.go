package server

import (
	"encoding/json"
	"io"
	"net/http"
	"strconv"

	"github.com/clbx/cosmic/internal/asm"
)

func jsonOK(w http.ResponseWriter, v any) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(v)
}

func jsonErr(w http.ResponseWriter, msg string, code int) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(code)
	json.NewEncoder(w).Encode(map[string]string{"error": msg})
}

// POST /api/load?origin=XXXX — body is raw binary
func (s *Server) handleLoad(w http.ResponseWriter, r *http.Request) {
	originStr := r.URL.Query().Get("origin")
	var origin uint64
	if originStr != "" {
		var err error
		origin, err = strconv.ParseUint(originStr, 16, 16)
		if err != nil {
			jsonErr(w, "invalid origin address", http.StatusBadRequest)
			return
		}
	}
	data, err := io.ReadAll(r.Body)
	if err != nil {
		jsonErr(w, "failed to read body", http.StatusBadRequest)
		return
	}
	s.mu.Lock()
	s.cpu.LoadROM(data, uint16(origin))
	s.mu.Unlock()
	jsonOK(w, map[string]string{"status": "loaded"})
}

// POST /api/reset
func (s *Server) handleReset(w http.ResponseWriter, r *http.Request) {
	s.mu.Lock()
	s.stopRunLoop()
	s.cpu.Reset()
	snap := s.cpu.Snapshot()
	s.mu.Unlock()
	jsonOK(w, snap)
}

// POST /api/resetmemory
func (s *Server) handleResetMemory(w http.ResponseWriter, r *http.Request) {
	s.mu.Lock()
	s.stopRunLoop()
	s.cpu.ResetMemory()
	snap := s.cpu.Snapshot()
	s.mu.Unlock()
	jsonOK(w, snap)
}

// POST /api/run?hz=N — start continuous execution at N Hz (omit for unconstrained)
func (s *Server) handleRun(w http.ResponseWriter, r *http.Request) {
	hz := 0
	if hzStr := r.URL.Query().Get("hz"); hzStr != "" {
		v, err := strconv.Atoi(hzStr)
		if err != nil || v < 0 {
			jsonErr(w, "hz must be a non-negative integer", http.StatusBadRequest)
			return
		}
		hz = v
	}

	s.mu.Lock()
	s.stopRunLoop() // stop any existing run first
	s.stopCh = make(chan struct{})
	stopCh := s.stopCh
	s.running = true
	s.mu.Unlock()

	go s.runLoop(stopCh, hz)
	jsonOK(w, map[string]any{"status": "running", "hz": hz})
}

// POST /api/stop
func (s *Server) handleStop(w http.ResponseWriter, r *http.Request) {
	s.mu.Lock()
	s.stopRunLoop()
	snap := s.cpu.Snapshot()
	s.mu.Unlock()
	jsonOK(w, snap)
}

// POST /api/step — execute one cycle, return updated state
func (s *Server) handleStep(w http.ResponseWriter, r *http.Request) {
	s.mu.Lock()
	if s.running {
		s.mu.Unlock()
		jsonErr(w, "CPU is running; stop it before stepping", http.StatusConflict)
		return
	}
	s.cpu.Cycle()
	snap := s.cpu.Snapshot()
	s.mu.Unlock()
	jsonOK(w, snap)
}

// GET /api/state
func (s *Server) handleState(w http.ResponseWriter, r *http.Request) {
	s.mu.Lock()
	snap := s.cpu.Snapshot()
	running := s.running
	s.mu.Unlock()
	jsonOK(w, map[string]any{"state": snap, "running": running})
}

// GET /api/memory?from=0000&to=00FF
func (s *Server) handleMemory(w http.ResponseWriter, r *http.Request) {
	fromStr := r.URL.Query().Get("from")
	toStr := r.URL.Query().Get("to")

	from, _ := strconv.ParseUint(fromStr, 16, 16)
	to, _ := strconv.ParseUint(toStr, 16, 16)
	if to == 0 || to < from {
		to = from + 0xFF
	}
	if to > 0xFFFF {
		to = 0xFFFF
	}

	s.mu.Lock()
	raw := make([]byte, to-from+1)
	copy(raw, s.cpu.Memory[from:to+1])
	s.mu.Unlock()

	// []byte encodes as base64 in Go's JSON; convert to []int for a numeric array.
	ints := make([]int, len(raw))
	for i, b := range raw {
		ints[i] = int(b)
	}
	jsonOK(w, map[string]any{
		"from": from,
		"to":   to,
		"data": ints,
	})
}

// GET /api/memory/dump?from=0000&to=FFFF — returns raw binary for download
func (s *Server) handleMemoryDump(w http.ResponseWriter, r *http.Request) {
	fromStr := r.URL.Query().Get("from")
	toStr := r.URL.Query().Get("to")

	from, _ := strconv.ParseUint(fromStr, 16, 16)
	to, _ := strconv.ParseUint(toStr, 16, 16)
	if to == 0 || to < from {
		to = 0xFFFF
	}
	if to > 0xFFFF {
		to = 0xFFFF
	}

	s.mu.Lock()
	raw := make([]byte, to-from+1)
	copy(raw, s.cpu.Memory[from:to+1])
	s.mu.Unlock()

	filename := "memory_" + fromStr + "_" + toStr + ".bin"
	w.Header().Set("Content-Type", "application/octet-stream")
	w.Header().Set("Content-Disposition", "attachment; filename="+filename)
	w.Write(raw)
}

// POST /api/memory — JSON body: {"address": "1234", "data": [1, 2, 3]}
func (s *Server) handleMemoryWrite(w http.ResponseWriter, r *http.Request) {
	var req struct {
		Address string  `json:"address"`
		Data    []uint8 `json:"data"`
	}
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		jsonErr(w, "invalid body", http.StatusBadRequest)
		return
	}
	addr, err := strconv.ParseUint(req.Address, 16, 16)
	if err != nil {
		jsonErr(w, "invalid address", http.StatusBadRequest)
		return
	}
	s.mu.Lock()
	for i, b := range req.Data {
		s.cpu.Write(uint16(addr)+uint16(i), b)
	}
	s.mu.Unlock()
	jsonOK(w, map[string]string{"status": "written"})
}

// POST /api/assemble — body is plain-text assembly source
func (s *Server) handleAssemble(w http.ResponseWriter, r *http.Request) {
	src, err := io.ReadAll(r.Body)
	if err != nil {
		jsonErr(w, "failed to read body", http.StatusBadRequest)
		return
	}
	result, aErr := asm.Assemble(string(src))
	if aErr != nil {
		jsonErr(w, aErr.Error(), http.StatusUnprocessableEntity)
		return
	}
	s.mu.Lock()
	s.cpu.LoadROM(result.Binary, 0x0000)
	s.mu.Unlock()
	jsonOK(w, map[string]any{
		"bytes":    len(result.Binary),
		"warnings": result.Warnings,
	})
}
