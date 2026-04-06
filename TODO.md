# Cosmic TODO

## CPU — `internal/cpu/instructions.go`
- [x] All opcodes implemented (NOP, HCF, PUSH, POP, SWP, CALL, RET)
- [x] ADD / ADDX, SUB / SUBX, MUL / MULX, DIV / DIVX
- [x] MOV / MOVX (all addressing mode combinations)
- [x] SHL / SHLX, SHR / SHRX
- [x] AND, OR, XOR
- [x] CMP / CMPX
- [x] All jumps (JMP, JZS, JNZ, JCS, JNC, JOS, JNO, JNS, JNN, JLS, JNL, JES)
- [x] Status flag ops (CSF, CZF/SZF, CNF/SNF, CCF/SCF, COF/SOF, CLF/SLF, CIF/SIF, CEF)
- [x] INC / INCX, DEC / DECX
- [x] Interrupt handling — `LPI()`, `HPI()`

## Assembler — `internal/asm/assembler.go`
- [ ] Tokenizer / line parser
- [ ] Label collection (pass 1)
- [ ] Opcode + operand encoding (pass 2)
- [ ] Addressing mode detection (IMP / IMM `#` / ABS / IND / REG `R0`–`R7`)
- [ ] Directive support (`.org`, `.db`, `.dw`)
- [ ] Error reporting with line numbers

## Server — `internal/server/`
- [x] CPU run loop goroutine with start/stop channel
- [x] Configurable clock speed (Hz) — `?hz=` query param on `/api/run`
- [x] WebSocket upgrade + 60fps state push (`ws.go`)
- [x] WebSocket inbound key events → write to `Memory[0xC400]`
- [x] PGU frame encoding (RGBA → PNG → base64) and push over WebSocket
- [ ] Breakpoint support — `POST /api/breakpoint`, halt run loop when PC matches

## PGU — `internal/pgu/pgu.go`
- [ ] Multi-byte color modes (2-byte and 4-byte per pixel)
- [ ] Palette/config file loader (parse the `pgu/` config format from `old/`)
- [ ] PNG encoding of rendered frame for WebSocket delivery

## Frontend — `frontend/`
- [ ] Memory editor: inline cell editing (click a byte to edit it)
- [ ] Memory editor: auto-refresh during run mode
- [ ] PGU canvas: render frames received over WebSocket
- [ ] Execution speed display (Hz counter)
- [ ] Breakpoint UI — click an address in memory editor to set/clear
- [ ] Load binary: set origin address before load
- [ ] Keyboard input forwarding over WebSocket
