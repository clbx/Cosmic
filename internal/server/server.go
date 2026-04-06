package server

import (
	"net/http"
	"runtime"
	"sync"
	"time"

	"github.com/clbx/cosmic/internal/cpu"
	"github.com/clbx/cosmic/internal/pgu"
)

// Server holds shared state and the HTTP mux.
type Server struct {
	mu      sync.Mutex
	mux     *http.ServeMux
	cpu     *cpu.CPU
	pgu     *pgu.PGU
	running bool
	stopCh  chan struct{}
}

// New creates a Server, wires up routes, and returns it as an http.Handler.
func New() *Server {
	s := &Server{
		mux:    http.NewServeMux(),
		cpu:    cpu.New(),
		pgu:    pgu.New(pgu.DefaultConfig),
		stopCh: make(chan struct{}),
	}
	s.routes()
	return s
}

func (s *Server) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	s.mux.ServeHTTP(w, r)
}

// routes registers all HTTP endpoints.
func (s *Server) routes() {
	// Static frontend files
	s.mux.Handle("/", http.FileServer(http.Dir("frontend/dist")))

	// REST — CPU control
	s.mux.HandleFunc("POST /api/load", s.handleLoad)
	s.mux.HandleFunc("POST /api/reset", s.handleReset)
	s.mux.HandleFunc("POST /api/resetmemory", s.handleResetMemory)
	s.mux.HandleFunc("POST /api/run", s.handleRun)
	s.mux.HandleFunc("POST /api/stop", s.handleStop)
	s.mux.HandleFunc("POST /api/step", s.handleStep)

	// REST — introspection
	s.mux.HandleFunc("GET /api/state", s.handleState)
	s.mux.HandleFunc("GET /api/memory", s.handleMemory)
	s.mux.HandleFunc("POST /api/memory", s.handleMemoryWrite)
	s.mux.HandleFunc("GET /api/memory/dump", s.handleMemoryDump)
	s.mux.HandleFunc("POST /api/assemble", s.handleAssemble)

	// WebSocket — real-time state push
	s.mux.HandleFunc("/ws", s.handleWS)
}

// stopRunLoop stops any in-progress run. Must be called with s.mu held.
func (s *Server) stopRunLoop() {
	if s.running {
		close(s.stopCh)
		s.running = false
	}
}

// runLoop executes CPU cycles continuously until stopped or the CPU halts.
// hz=0 means run as fast as possible; hz>0 throttles to that frequency.
func (s *Server) runLoop(stopCh <-chan struct{}, hz int) {
	defer func() {
		s.mu.Lock()
		s.running = false
		s.mu.Unlock()
	}()

	cycle := func() (alive bool) {
		s.mu.Lock()
		alive = s.cpu.Cycle()
		s.mu.Unlock()
		return
	}

	if hz <= 0 {
		// Unconstrained: run as fast as possible, yielding periodically
		// so HTTP handlers can acquire the mutex between cycles.
		for {
			select {
			case <-stopCh:
				return
			default:
				if !cycle() {
					return
				}
				runtime.Gosched()
			}
		}
	}

	ticker := time.NewTicker(time.Second / time.Duration(hz))
	defer ticker.Stop()
	for {
		select {
		case <-stopCh:
			return
		case <-ticker.C:
			if !cycle() {
				return
			}
		}
	}
}
