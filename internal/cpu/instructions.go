package cpu

// ── Helpers ──────────────────────────────────────────────────────────────────

// read16 reads a big-endian 16-bit value from memory.
func (c *CPU) read16(addr uint16) uint16 {
	return uint16(c.Memory[addr])<<8 | uint16(c.Memory[addr+1])
}

// write16 writes a big-endian 16-bit value to memory.
func (c *CPU) write16(addr uint16, val uint16) {
	c.Memory[addr] = uint8(val >> 8)
	c.Memory[addr+1] = uint8(val)
}

// pairBase normalises a register index to the even base of its 16-bit pair.
func pairBase(reg uint8) uint8 {
	if reg%2 != 0 {
		return reg - 1
	}
	return reg
}

// regPairVal returns the 16-bit value of the register pair that contains idx.
func (c *CPU) regPairVal(idx uint8) uint16 {
	b := pairBase(idx)
	return uint16(c.Registers[b])<<8 | uint16(c.Registers[b+1])
}

// setRegPair stores a 16-bit value into the register pair that contains idx.
func (c *CPU) setRegPair(idx uint8, val uint16) {
	b := pairBase(idx)
	c.Registers[b] = uint8(val >> 8)
	c.Registers[b+1] = uint8(val)
}

// setArith8 sets N/C/O/Z flags after an 8-bit arithmetic operation.
// acc and data are the original operands; result is the raw (possibly >8-bit) result.
func setArith8(c *CPU, acc, data uint8, result uint) {
	c.setFlag(FlagN, result >= 0x80)
	c.setFlag(FlagC, result > 0xFF)
	c.setFlag(FlagO, (acc^uint8(result))&(data^uint8(result))&0x80 != 0)
	c.setFlag(FlagZ, result&0xFF == 0)
}

// setArith16 sets N/C/O/Z flags after a 16-bit arithmetic operation.
func setArith16(c *CPU, acc, data uint16, result uint) {
	c.setFlag(FlagN, result >= 0x8000)
	c.setFlag(FlagC, result > 0xFFFF)
	c.setFlag(FlagO, (acc^uint16(result))&(data^uint16(result))&0x8000 != 0)
	c.setFlag(FlagZ, result&0xFFFF == 0)
}

// ── Interrupt handlers ────────────────────────────────────────────────────────

// LPI triggers a low-priority interrupt (maskable via I flag).
func (c *CPU) LPI() {
	if c.getFlag(FlagI) {
		return
	}
	c.Memory[c.SP] = uint8(c.PC & 0x00FF)
	c.SP--
	c.Memory[c.SP] = uint8((c.PC & 0xFF00) >> 8)
	c.SP--
	c.PC = c.read16(0xFFF2)
}

// HPI triggers a high-priority interrupt (non-maskable).
func (c *CPU) HPI() {
	c.Memory[c.SP] = uint8(c.PC & 0x00FF)
	c.SP--
	c.Memory[c.SP] = uint8((c.PC & 0xFF00) >> 8)
	c.SP--
	c.PC = c.read16(0xFFF4)
}

// ── Instruction set ───────────────────────────────────────────────────────────

// instruction holds the handler and metadata for one opcode.
type instruction struct {
	mnemonic string
	bytes    int // informational only; execute() is responsible for PC advancement
	execute  func(c *CPU)
}

// instructionSet is the full 256-entry opcode table.
var instructionSet [256]instruction

// buildInstructionSet populates instructionSet. Called once on CPU init.
func (c *CPU) buildInstructionSet() {
	// Default: unknown opcode advances PC by 1
	for i := range instructionSet {
		instructionSet[i] = instruction{"???", 1, func(c *CPU) { c.PC++ }}
	}

	// ── 0x00–0x08  Control ────────────────────────────────────────────────────

	instructionSet[0x00] = instruction{"NOP", 1, func(c *CPU) {
		c.PC++
	}}

	instructionSet[0x01] = instruction{"HCF", 1, func(c *CPU) {
		c.Halted = true
	}}

	// PUSH  – push accumulator (R0) onto stack
	instructionSet[0x02] = instruction{"PUSH", 1, func(c *CPU) {
		c.Memory[c.SP] = c.Registers[0]
		c.SP--
		c.PC++
	}}

	// POP  – pop top of stack into accumulator (R0)
	instructionSet[0x03] = instruction{"POP", 1, func(c *CPU) {
		c.SP++
		c.Registers[0] = c.Memory[c.SP]
		c.PC++
	}}

	// SWP  reg1, reg2  – swap two registers
	instructionSet[0x04] = instruction{"SWP", 3, func(c *CPU) {
		r1 := c.Memory[c.PC+1]
		r2 := c.Memory[c.PC+2]
		c.Registers[r1], c.Registers[r2] = c.Registers[r2], c.Registers[r1]
		c.PC += 3
	}}

	// CALL #imm  – push PC, jump to immediate 16-bit address
	instructionSet[0x05] = instruction{"CALL #imm", 3, func(c *CPU) {
		target := c.read16(c.PC + 1)
		c.Memory[c.SP] = uint8(c.PC & 0xFF)
		c.SP--
		c.Memory[c.SP] = uint8((c.PC >> 8) & 0xFF)
		c.SP--
		c.PC = target
	}}

	// CALL abs
	instructionSet[0x06] = instruction{"CALL abs", 3, func(c *CPU) {
		target := c.read16(c.read16(c.PC + 1))
		c.Memory[c.SP] = uint8(c.PC & 0xFF)
		c.SP--
		c.Memory[c.SP] = uint8((c.PC >> 8) & 0xFF)
		c.SP--
		c.PC = target
	}}

	// CALL @ind
	instructionSet[0x07] = instruction{"CALL @ind", 3, func(c *CPU) {
		ptr := c.read16(c.PC + 1)
		target := c.read16(c.read16(ptr))
		c.Memory[c.SP] = uint8(c.PC & 0xFF)
		c.SP--
		c.Memory[c.SP] = uint8((c.PC >> 8) & 0xFF)
		c.SP--
		c.PC = target
	}}

	// RET  – restore PC from stack, skip 3 bytes past the CALL instruction
	instructionSet[0x08] = instruction{"RET", 3, func(c *CPU) {
		c.PC = c.read16(c.SP+1) + 3
		c.SP += 2
	}}

	// ── 0x10–0x17  ADD / ADDX ─────────────────────────────────────────────────

	// ADD #imm
	instructionSet[0x10] = instruction{"ADD #imm", 2, func(c *CPU) {
		acc := c.Registers[0]
		data := c.Memory[c.PC+1]
		result := uint(acc) + uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.PC += 2
	}}

	// ADD abs / ADD @ind
	addMem8 := func(c *CPU, src uint16, size uint16) {
		acc := c.Registers[0]
		data := c.Memory[src]
		result := uint(acc) + uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.PC += size
	}
	instructionSet[0x11] = instruction{"ADD abs", 3, func(c *CPU) { addMem8(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x12] = instruction{"ADD @ind", 3, func(c *CPU) { addMem8(c, c.read16(c.read16(c.PC+1)), 3) }}

	// ADD Rx (register)
	instructionSet[0x13] = instruction{"ADD Rx", 2, func(c *CPU) {
		acc := c.Registers[0]
		data := c.Registers[c.Memory[c.PC+1]]
		result := uint(acc) + uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.PC += 2
	}}

	// ADDX #imm  (16-bit, AB pair)
	addX := func(c *CPU, data uint16, size uint16) {
		acc := c.regPairVal(0)
		result := uint(acc) + uint(data)
		setArith16(c, acc, data, result)
		c.setRegPair(0, uint16(result))
		c.PC += size
	}
	instructionSet[0x14] = instruction{"ADDX #imm", 3, func(c *CPU) { addX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x15] = instruction{"ADDX abs", 3, func(c *CPU) { addX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x16] = instruction{"ADDX @ind", 3, func(c *CPU) {
		addX(c, c.read16(c.read16(c.read16(c.PC+1))), 3)
	}}
	instructionSet[0x17] = instruction{"ADDX Rx", 2, func(c *CPU) { addX(c, c.regPairVal(c.Memory[c.PC+1]), 2) }}

	// ── 0x18–0x1F  SUB / SUBX ────────────────────────────────────────────────

	subMem8 := func(c *CPU, src uint16, size uint16) {
		acc := c.Registers[0]
		data := c.Memory[src]
		result := uint(acc) - uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.PC += size
	}
	instructionSet[0x18] = instruction{"SUB #imm", 2, func(c *CPU) { subMem8(c, c.PC+1, 2) }}
	instructionSet[0x19] = instruction{"SUB abs", 3, func(c *CPU) { subMem8(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x1A] = instruction{"SUB @ind", 3, func(c *CPU) { subMem8(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x1B] = instruction{"SUB Rx", 2, func(c *CPU) {
		acc := c.Registers[0]
		data := c.Registers[c.Memory[c.PC+1]]
		result := uint(acc) - uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.PC += 2
	}}

	subX := func(c *CPU, data uint16, size uint16) {
		acc := c.regPairVal(0)
		result := uint(acc) - uint(data)
		setArith16(c, acc, data, result)
		c.setRegPair(0, uint16(result))
		c.PC += size
	}
	instructionSet[0x1C] = instruction{"SUBX #imm", 3, func(c *CPU) { subX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x1D] = instruction{"SUBX abs", 3, func(c *CPU) { subX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x1E] = instruction{"SUBX @ind", 3, func(c *CPU) {
		subX(c, c.read16(c.read16(c.read16(c.PC+1))), 3)
	}}
	instructionSet[0x1F] = instruction{"SUBX Rx", 2, func(c *CPU) { subX(c, c.regPairVal(c.Memory[c.PC+1]), 2) }}

	// ── 0x20–0x27  MUL / MULX ────────────────────────────────────────────────

	mulMem8 := func(c *CPU, src uint16, size uint16) {
		acc := c.Registers[0]
		data := c.Memory[src]
		result := uint(acc) * uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.PC += size
	}
	instructionSet[0x20] = instruction{"MUL #imm", 2, func(c *CPU) { mulMem8(c, c.PC+1, 2) }}
	instructionSet[0x21] = instruction{"MUL abs", 3, func(c *CPU) { mulMem8(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x22] = instruction{"MUL @ind", 3, func(c *CPU) { mulMem8(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x23] = instruction{"MUL Rx", 2, func(c *CPU) {
		acc := c.Registers[0]
		data := c.Registers[c.Memory[c.PC+1]]
		result := uint(acc) * uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.PC += 2
	}}

	mulX := func(c *CPU, data uint16, size uint16) {
		acc := c.regPairVal(0)
		result := uint(acc) * uint(data)
		setArith16(c, acc, data, result)
		c.setRegPair(0, uint16(result))
		c.PC += size
	}
	instructionSet[0x24] = instruction{"MULX #imm", 3, func(c *CPU) { mulX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x25] = instruction{"MULX abs", 3, func(c *CPU) { mulX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x26] = instruction{"MULX @ind", 3, func(c *CPU) {
		mulX(c, c.read16(c.read16(c.read16(c.PC+1))), 3)
	}}
	instructionSet[0x27] = instruction{"MULX Rx", 2, func(c *CPU) { mulX(c, c.regPairVal(c.Memory[c.PC+1]), 2) }}

	// ── 0x28–0x2F  DIV / DIVX ────────────────────────────────────────────────

	divMem8 := func(c *CPU, src uint16, size uint16) {
		data := c.Memory[src]
		if data == 0 {
			c.setFlag(FlagE, true)
			c.PC += size
			return
		}
		acc := c.Registers[0]
		result := uint(acc) / uint(data)
		remainder := uint(acc) % uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.Registers[1] = uint8(remainder) // remainder → B
		c.PC += size
	}
	instructionSet[0x28] = instruction{"DIV #imm", 2, func(c *CPU) { divMem8(c, c.PC+1, 2) }}
	instructionSet[0x29] = instruction{"DIV abs", 3, func(c *CPU) { divMem8(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x2A] = instruction{"DIV @ind", 3, func(c *CPU) { divMem8(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x2B] = instruction{"DIV Rx", 2, func(c *CPU) {
		data := c.Registers[c.Memory[c.PC+1]]
		if data == 0 {
			c.setFlag(FlagE, true)
			c.PC += 2
			return
		}
		acc := c.Registers[0]
		result := uint(acc) / uint(data)
		remainder := uint(acc) % uint(data)
		setArith8(c, acc, data, result)
		c.Registers[0] = uint8(result)
		c.Registers[1] = uint8(remainder) // remainder → B
		c.PC += 2
	}}

	divX := func(c *CPU, data uint16, size uint16) {
		if data == 0 {
			c.setFlag(FlagE, true)
			c.PC += size
			return
		}
		acc := c.regPairVal(0)
		result := uint(acc) / uint(data)
		remainder := uint(acc) % uint(data)
		setArith16(c, acc, data, result)
		c.setRegPair(0, uint16(result))
		c.setRegPair(2, uint16(remainder)) // remainder → CD
		c.PC += size
	}
	instructionSet[0x2C] = instruction{"DIVX #imm", 3, func(c *CPU) { divX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x2D] = instruction{"DIVX abs", 3, func(c *CPU) { divX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x2E] = instruction{"DIVX @ind", 3, func(c *CPU) {
		divX(c, c.read16(c.read16(c.read16(c.PC+1))), 3)
	}}
	instructionSet[0x2F] = instruction{"DIVX Rx", 2, func(c *CPU) { divX(c, c.regPairVal(c.Memory[c.PC+1]), 2) }}

	// ── 0x30–0x3B  MOV (8-bit) ───────────────────────────────────────────────
	//
	// Destination types: A=Absolute, I=Indirect, R=Register
	// Source types:      I=Immediate, A=Absolute, I=Indirect, R=Register
	// Mnemonic: MOV<dst><src>

	// MOV #imm, abs  – write immediate byte to absolute address
	instructionSet[0x30] = instruction{"MOV #imm, abs", 4, func(c *CPU) {
		val := c.Memory[c.PC+1]
		dst := c.read16(c.PC + 2)
		c.Memory[dst] = val
		c.PC += 4
	}}

	// MOV abs, abs  /  MOV @ind, abs
	instructionSet[0x31] = instruction{"MOV abs, abs", 5, func(c *CPU) {
		src := c.read16(c.PC + 1)
		dst := c.read16(c.PC + 3)
		c.Memory[dst] = c.Memory[src]
		c.PC += 5
	}}
	instructionSet[0x32] = instruction{"MOV @ind, abs", 5, func(c *CPU) {
		src := c.read16(c.read16(c.PC + 1))
		dst := c.read16(c.PC + 3)
		c.Memory[dst] = c.Memory[src]
		c.PC += 5
	}}

	// MOV Rx, abs
	instructionSet[0x33] = instruction{"MOV Rx, abs", 4, func(c *CPU) {
		reg := c.Memory[c.PC+1]
		dst := c.read16(c.PC + 2)
		c.Memory[dst] = c.Registers[reg]
		c.PC += 4
	}}

	// MOV #imm, @ind
	instructionSet[0x34] = instruction{"MOV #imm, @ind", 4, func(c *CPU) {
		val := c.Memory[c.PC+1]
		preDst := c.read16(c.PC + 2)
		dst := c.read16(preDst)
		c.Memory[dst] = val
		c.PC += 4
	}}

	// MOV abs, @ind  /  MOV @ind, @ind
	instructionSet[0x35] = instruction{"MOV abs, @ind", 5, func(c *CPU) {
		src := c.read16(c.PC + 1)
		preDst := c.read16(c.PC + 3)
		dst := c.read16(preDst)
		c.Memory[dst] = c.Memory[src]
		c.PC += 5
	}}
	instructionSet[0x36] = instruction{"MOV @ind, @ind", 5, func(c *CPU) {
		src := c.read16(c.read16(c.PC + 1))
		preDst := c.read16(c.PC + 3)
		dst := c.read16(preDst)
		c.Memory[dst] = c.Memory[src]
		c.PC += 5
	}}

	// MOV Rx, @ind
	instructionSet[0x37] = instruction{"MOV Rx, @ind", 4, func(c *CPU) {
		reg := c.Memory[c.PC+1]
		preDst := c.read16(c.PC + 2)
		dst := c.read16(preDst)
		c.Memory[dst] = c.Registers[reg]
		c.PC += 4
	}}

	// MOV #imm, Rx  – load immediate into register (encoding: imm, reg)
	instructionSet[0x38] = instruction{"MOV #imm, Rx", 3, func(c *CPU) {
		val := c.Memory[c.PC+1]
		reg := c.Memory[c.PC+2]
		c.Registers[reg] = val
		c.PC += 3
	}}

	// MOV abs, Rx  /  MOV @ind, Rx
	instructionSet[0x39] = instruction{"MOV abs, Rx", 4, func(c *CPU) {
		src := c.read16(c.PC + 1)
		reg := c.Memory[c.PC+3]
		c.Registers[reg] = c.Memory[src]
		c.PC += 4
	}}
	instructionSet[0x3A] = instruction{"MOV @ind, Rx", 4, func(c *CPU) {
		src := c.read16(c.read16(c.PC + 1))
		reg := c.Memory[c.PC+3]
		c.Registers[reg] = c.Memory[src]
		c.PC += 4
	}}

	// MOV Rx, Ry
	instructionSet[0x3B] = instruction{"MOV Rx, Ry", 3, func(c *CPU) {
		src := c.Memory[c.PC+1]
		dst := c.Memory[c.PC+2]
		c.Registers[dst] = c.Registers[src]
		c.PC += 3
	}}

	// ── 0x3C–0x3F  SHL (8-bit accumulator) ───────────────────────────────────

	shl8 := func(c *CPU, shift uint8, size uint16) {
		if shift > 8 {
			shift = 8
		}
		shifted := uint16(c.Registers[0]) << shift
		c.setFlag(FlagC, shifted&0xFF00 != 0)
		c.Registers[0] = uint8(shifted)
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += size
	}
	instructionSet[0x3C] = instruction{"SHL #imm", 2, func(c *CPU) { shl8(c, c.Memory[c.PC+1], 2) }}
	instructionSet[0x3D] = instruction{"SHL abs", 3, func(c *CPU) { shl8(c, c.Memory[c.read16(c.PC+1)], 3) }}
	instructionSet[0x3E] = instruction{"SHL @ind", 3, func(c *CPU) {
		shl8(c, c.Memory[c.read16(c.read16(c.PC+1))], 3)
	}}
	instructionSet[0x3F] = instruction{"SHL Rx", 2, func(c *CPU) {
		shl8(c, c.Registers[c.Memory[c.PC+1]], 2)
	}}

	// ── 0x40–0x4B  MOVX (16-bit) ─────────────────────────────────────────────

	// MOVX #imm, abs  – write 16-bit immediate to absolute address
	instructionSet[0x40] = instruction{"MOVX #imm, abs", 5, func(c *CPU) {
		valHi := c.Memory[c.PC+1]
		valLo := c.Memory[c.PC+2]
		dst := c.read16(c.PC + 3)
		c.Memory[dst] = valHi
		c.Memory[dst+1] = valLo
		c.PC += 5
	}}

	// MOVX abs, abs  /  MOVX @ind, abs
	instructionSet[0x41] = instruction{"MOVX abs, abs", 5, func(c *CPU) {
		src := c.read16(c.PC + 1)
		dst := c.read16(c.PC + 3)
		c.Memory[dst] = c.Memory[src]
		c.Memory[dst+1] = c.Memory[src+1]
		c.PC += 5
	}}
	instructionSet[0x42] = instruction{"MOVX @ind, abs", 5, func(c *CPU) {
		src := c.read16(c.read16(c.PC + 1))
		dst := c.read16(c.PC + 3)
		c.Memory[dst] = c.Memory[src]
		c.Memory[dst+1] = c.Memory[src+1]
		c.PC += 5
	}}

	// MOVX Rx, abs
	instructionSet[0x43] = instruction{"MOVX Rx, abs", 4, func(c *CPU) {
		reg := pairBase(c.Memory[c.PC+1])
		dst := c.read16(c.PC + 2)
		c.Memory[dst] = c.Registers[reg]
		c.Memory[dst+1] = c.Registers[reg+1]
		c.PC += 4
	}}

	// MOVX #imm, @ind
	instructionSet[0x44] = instruction{"MOVX #imm, @ind", 5, func(c *CPU) {
		valHi := c.Memory[c.PC+1]
		valLo := c.Memory[c.PC+2]
		preDst := c.read16(c.PC + 3)
		dst := c.read16(preDst)
		c.Memory[dst] = valHi
		c.Memory[dst+1] = valLo
		c.PC += 5
	}}

	// MOVX abs, @ind  /  MOVX @ind, @ind
	instructionSet[0x45] = instruction{"MOVX abs, @ind", 5, func(c *CPU) {
		src := c.read16(c.PC + 1)
		preDst := c.read16(c.PC + 3)
		dst := c.read16(preDst)
		c.Memory[dst] = c.Memory[src]
		c.Memory[dst+1] = c.Memory[src+1]
		c.PC += 5
	}}
	instructionSet[0x46] = instruction{"MOVX @ind, @ind", 5, func(c *CPU) {
		src := c.read16(c.read16(c.PC + 1))
		preDst := c.read16(c.PC + 3)
		dst := c.read16(preDst)
		c.Memory[dst] = c.Memory[src]
		c.Memory[dst+1] = c.Memory[src+1]
		c.PC += 5
	}}

	// MOVX Rx, @ind
	instructionSet[0x47] = instruction{"MOVX Rx, @ind", 4, func(c *CPU) {
		reg := pairBase(c.Memory[c.PC+1])
		preDst := c.read16(c.PC + 2)
		dst := c.read16(preDst)
		c.Memory[dst] = c.Registers[reg]
		c.Memory[dst+1] = c.Registers[reg+1]
		c.PC += 4
	}}

	// MOVX #imm, Rx
	instructionSet[0x48] = instruction{"MOVX #imm, Rx", 4, func(c *CPU) {
		valHi := c.Memory[c.PC+1]
		valLo := c.Memory[c.PC+2]
		reg := pairBase(c.Memory[c.PC+3])
		c.Registers[reg] = valHi
		c.Registers[reg+1] = valLo
		c.PC += 4
	}}

	// MOVX abs, Rx  /  MOVX @ind, Rx
	instructionSet[0x49] = instruction{"MOVX abs, Rx", 4, func(c *CPU) {
		src := c.read16(c.PC + 1)
		reg := pairBase(c.Memory[c.PC+3])
		c.Registers[reg] = c.Memory[src]
		c.Registers[reg+1] = c.Memory[src+1]
		c.PC += 4
	}}
	instructionSet[0x4A] = instruction{"MOVX @ind, Rx", 4, func(c *CPU) {
		src := c.read16(c.read16(c.PC + 1))
		reg := pairBase(c.Memory[c.PC+3])
		c.Registers[reg] = c.Memory[src]
		c.Registers[reg+1] = c.Memory[src+1]
		c.PC += 4
	}}

	// MOVX Rx, Ry
	instructionSet[0x4B] = instruction{"MOVX Rx, Ry", 3, func(c *CPU) {
		src := pairBase(c.Memory[c.PC+1])
		dst := pairBase(c.Memory[c.PC+2])
		c.Registers[dst] = c.Registers[src]
		c.Registers[dst+1] = c.Registers[src+1]
		c.PC += 3
	}}

	// ── 0x4C–0x4F  SHLX (16-bit accumulator) ─────────────────────────────────

	shlX := func(c *CPU, shift uint16, size uint16) {
		if shift > 16 {
			shift = 16
		}
		shifted := uint32(c.regPairVal(0)) << shift
		c.setFlag(FlagC, shifted&0xFFFF0000 != 0)
		c.setRegPair(0, uint16(shifted))
		c.setFlag(FlagZ, uint16(shifted) == 0)
		c.PC += size
	}
	instructionSet[0x4C] = instruction{"SHLX #imm", 3, func(c *CPU) { shlX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x4D] = instruction{"SHLX abs", 3, func(c *CPU) { shlX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x4E] = instruction{"SHLX @ind", 3, func(c *CPU) {
		shlX(c, c.read16(c.read16(c.read16(c.PC+1))), 3)
	}}
	instructionSet[0x4F] = instruction{"SHLX Rx", 2, func(c *CPU) {
		shlX(c, c.regPairVal(c.Memory[c.PC+1]), 2)
	}}

	// ── 0x50–0x5B  AND / OR / XOR ────────────────────────────────────────────

	instructionSet[0x50] = instruction{"AND #imm", 2, func(c *CPU) {
		c.Registers[0] &= c.Memory[c.PC+1]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 2
	}}
	instructionSet[0x51] = instruction{"AND abs", 3, func(c *CPU) {
		c.Registers[0] &= c.Memory[c.read16(c.PC+1)]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 3
	}}
	instructionSet[0x52] = instruction{"AND @ind", 3, func(c *CPU) {
		c.Registers[0] &= c.Memory[c.read16(c.read16(c.PC+1))]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 3
	}}
	instructionSet[0x53] = instruction{"AND Rx", 2, func(c *CPU) {
		c.Registers[0] &= c.Registers[c.Memory[c.PC+1]]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 2
	}}

	instructionSet[0x54] = instruction{"OR #imm", 2, func(c *CPU) {
		c.Registers[0] |= c.Memory[c.PC+1]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 2
	}}
	instructionSet[0x55] = instruction{"OR abs", 3, func(c *CPU) {
		c.Registers[0] |= c.Memory[c.read16(c.PC+1)]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 3
	}}
	instructionSet[0x56] = instruction{"OR @ind", 3, func(c *CPU) {
		c.Registers[0] |= c.Memory[c.read16(c.read16(c.PC+1))]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 3
	}}
	instructionSet[0x57] = instruction{"OR Rx", 2, func(c *CPU) {
		c.Registers[0] |= c.Registers[c.Memory[c.PC+1]]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 2
	}}

	instructionSet[0x58] = instruction{"XOR #imm", 2, func(c *CPU) {
		c.Registers[0] ^= c.Memory[c.PC+1]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 2
	}}
	instructionSet[0x59] = instruction{"XOR abs", 3, func(c *CPU) {
		c.Registers[0] ^= c.Memory[c.read16(c.PC+1)]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 3
	}}
	instructionSet[0x5A] = instruction{"XOR @ind", 3, func(c *CPU) {
		c.Registers[0] ^= c.Memory[c.read16(c.read16(c.PC+1))]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 3
	}}
	instructionSet[0x5B] = instruction{"XOR Rx", 2, func(c *CPU) {
		c.Registers[0] ^= c.Registers[c.Memory[c.PC+1]]
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += 2
	}}

	// ── 0x5C–0x5F  SHR (8-bit accumulator) ──────────────────────────────────

	shr8 := func(c *CPU, shift uint8, size uint16) {
		c.Registers[0] >>= shift
		c.setFlag(FlagZ, c.Registers[0] == 0)
		c.PC += size
	}
	instructionSet[0x5C] = instruction{"SHR #imm", 2, func(c *CPU) { shr8(c, c.Memory[c.PC+1], 2) }}
	instructionSet[0x5D] = instruction{"SHR abs", 3, func(c *CPU) { shr8(c, c.Memory[c.read16(c.PC+1)], 3) }}
	instructionSet[0x5E] = instruction{"SHR @ind", 3, func(c *CPU) {
		shr8(c, c.Memory[c.read16(c.read16(c.PC+1))], 3)
	}}
	instructionSet[0x5F] = instruction{"SHR Rx", 2, func(c *CPU) {
		shr8(c, c.Registers[c.Memory[c.PC+1]], 2)
	}}

	// ── 0x60–0x67  CMP / CMPX ────────────────────────────────────────────────
	//
	// Note: CMP clears ALL flags before comparing. The original C++ only cleared
	// the L flag (via CLF), leaving other flags potentially stale — that is a bug
	// in the original that we deliberately fix here for correct behaviour.

	cmp8 := func(c *CPU, operand uint8, size uint16) {
		c.setFlag(FlagL, false) // CLF: only the Less flag is cleared before comparison
		acc := c.Registers[0]
		if acc == operand {
			c.setFlag(FlagZ, true)
		}
		if acc > operand { // unsigned: accumulator greater → carry set
			c.setFlag(FlagC, true)
		}
		if acc < operand { // unsigned: accumulator less → negative set
			c.setFlag(FlagN, true)
		}
		if int8(acc) > int8(operand) { // signed: accumulator greater → overflow set
			c.setFlag(FlagO, true)
		}
		if int8(acc) < int8(operand) { // signed: accumulator less → less set
			c.setFlag(FlagL, true)
		}
		c.PC += size
	}
	instructionSet[0x60] = instruction{"CMP #imm", 2, func(c *CPU) { cmp8(c, c.Memory[c.PC+1], 2) }}
	instructionSet[0x61] = instruction{"CMP abs", 3, func(c *CPU) { cmp8(c, c.Memory[c.read16(c.PC+1)], 3) }}
	instructionSet[0x62] = instruction{"CMP @ind", 3, func(c *CPU) {
		cmp8(c, c.Memory[c.read16(c.read16(c.PC+1))], 3)
	}}
	instructionSet[0x63] = instruction{"CMP Rx", 2, func(c *CPU) {
		cmp8(c, c.Registers[c.Memory[c.PC+1]], 2)
	}}

	cmpX := func(c *CPU, operand uint16, size uint16) {
		c.setFlag(FlagL, false) // CLF: only the Less flag is cleared before comparison
		acc := c.regPairVal(0)
		if acc == operand {
			c.setFlag(FlagZ, true)
		}
		if acc > operand {
			c.setFlag(FlagC, true)
		}
		if acc < operand {
			c.setFlag(FlagN, true)
		}
		if int16(acc) > int16(operand) {
			c.setFlag(FlagO, true)
		}
		if int16(acc) < int16(operand) {
			c.setFlag(FlagL, true)
		}
		c.PC += size
	}
	instructionSet[0x64] = instruction{"CMPX #imm", 3, func(c *CPU) { cmpX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x65] = instruction{"CMPX abs", 3, func(c *CPU) { cmpX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x66] = instruction{"CMPX @ind", 3, func(c *CPU) {
		cmpX(c, c.read16(c.read16(c.read16(c.PC+1))), 3)
	}}
	instructionSet[0x67] = instruction{"CMPX Rx", 2, func(c *CPU) {
		cmpX(c, c.regPairVal(c.Memory[c.PC+1]), 2)
	}}

	// ── 0x6C–0x6F  SHRX (16-bit accumulator) ─────────────────────────────────

	shrX := func(c *CPU, shift uint16, size uint16) {
		result := c.regPairVal(0) >> shift
		c.setRegPair(0, result)
		c.setFlag(FlagZ, result == 0)
		c.PC += size
	}
	instructionSet[0x6C] = instruction{"SHRX #imm", 3, func(c *CPU) { shrX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0x6D] = instruction{"SHRX abs", 3, func(c *CPU) { shrX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0x6E] = instruction{"SHRX @ind", 3, func(c *CPU) {
		shrX(c, c.read16(c.read16(c.read16(c.PC+1))), 3)
	}}
	instructionSet[0x6F] = instruction{"SHRX Rx", 2, func(c *CPU) {
		shrX(c, c.regPairVal(c.Memory[c.PC+1]), 2)
	}}

	// ── 0x70–0x9F  Jumps ──────────────────────────────────────────────────────
	//
	// Each jump family has 4 variants: IMM, ABS, IND, REG.
	// IMM/ABS/IND: 3-byte instruction — if not taken, advance PC by 3.
	// REG:         2-byte instruction — if not taken, advance PC by 2.

	// condJump executes a conditional branch.
	// taken: whether to branch; src: target address source; isReg: REG mode
	condJump := func(c *CPU, cond bool, src uint16, isReg bool) {
		if cond {
			if isReg {
				c.PC = c.regPairVal(c.Memory[c.PC+1])
			} else {
				c.PC = c.read16(src)
			}
		} else {
			if isReg {
				c.PC += 2
			} else {
				c.PC += 3
			}
		}
	}

	// JMP – unconditional
	instructionSet[0x70] = instruction{"JMP #imm", 3, func(c *CPU) { c.PC = c.read16(c.PC + 1) }}
	instructionSet[0x71] = instruction{"JMP abs", 3, func(c *CPU) { c.PC = c.read16(c.read16(c.PC + 1)) }}
	instructionSet[0x72] = instruction{"JMP @ind", 3, func(c *CPU) {
		c.PC = c.read16(c.read16(c.read16(c.PC + 1)))
	}}
	instructionSet[0x73] = instruction{"JMP Rx", 2, func(c *CPU) {
		c.PC = c.regPairVal(c.Memory[c.PC+1])
	}}

	// JZS – jump if zero set
	instructionSet[0x74] = instruction{"JZS #imm", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagZ), c.PC+1, false)
	}}
	instructionSet[0x75] = instruction{"JZS abs", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagZ), c.read16(c.PC+1), false)
	}}
	instructionSet[0x76] = instruction{"JZS @ind", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagZ), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x77] = instruction{"JZS Rx", 2, func(c *CPU) {
		condJump(c, c.getFlag(FlagZ), 0, true)
	}}

	// JNZ – jump if zero clear
	instructionSet[0x78] = instruction{"JNZ #imm", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagZ), c.PC+1, false)
	}}
	instructionSet[0x79] = instruction{"JNZ abs", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagZ), c.read16(c.PC+1), false)
	}}
	instructionSet[0x7A] = instruction{"JNZ @ind", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagZ), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x7B] = instruction{"JNZ Rx", 2, func(c *CPU) {
		condJump(c, !c.getFlag(FlagZ), 0, true)
	}}

	// JCS – jump if carry set
	instructionSet[0x7C] = instruction{"JCS #imm", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagC), c.PC+1, false)
	}}
	instructionSet[0x7D] = instruction{"JCS abs", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagC), c.read16(c.PC+1), false)
	}}
	instructionSet[0x7E] = instruction{"JCS @ind", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagC), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x7F] = instruction{"JCS Rx", 2, func(c *CPU) {
		condJump(c, c.getFlag(FlagC), 0, true)
	}}

	// JNC – jump if carry clear
	instructionSet[0x80] = instruction{"JNC #imm", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagC), c.PC+1, false)
	}}
	instructionSet[0x81] = instruction{"JNC abs", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagC), c.read16(c.PC+1), false)
	}}
	instructionSet[0x82] = instruction{"JNC @ind", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagC), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x83] = instruction{"JNC Rx", 2, func(c *CPU) {
		condJump(c, !c.getFlag(FlagC), 0, true)
	}}

	// JOS – jump if overflow set
	instructionSet[0x84] = instruction{"JOS #imm", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagO), c.PC+1, false)
	}}
	instructionSet[0x85] = instruction{"JOS abs", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagO), c.read16(c.PC+1), false)
	}}
	instructionSet[0x86] = instruction{"JOS @ind", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagO), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x87] = instruction{"JOS Rx", 2, func(c *CPU) {
		condJump(c, c.getFlag(FlagO), 0, true)
	}}

	// JNO – jump if overflow clear
	instructionSet[0x88] = instruction{"JNO #imm", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagO), c.PC+1, false)
	}}
	instructionSet[0x89] = instruction{"JNO abs", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagO), c.read16(c.PC+1), false)
	}}
	instructionSet[0x8A] = instruction{"JNO @ind", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagO), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x8B] = instruction{"JNO Rx", 2, func(c *CPU) {
		condJump(c, !c.getFlag(FlagO), 0, true)
	}}

	// JNS – jump if negative set
	instructionSet[0x8C] = instruction{"JNS #imm", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagN), c.PC+1, false)
	}}
	instructionSet[0x8D] = instruction{"JNS abs", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagN), c.read16(c.PC+1), false)
	}}
	instructionSet[0x8E] = instruction{"JNS @ind", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagN), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x8F] = instruction{"JNS Rx", 2, func(c *CPU) {
		condJump(c, c.getFlag(FlagN), 0, true)
	}}

	// JNN – jump if negative clear
	instructionSet[0x90] = instruction{"JNN #imm", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagN), c.PC+1, false)
	}}
	instructionSet[0x91] = instruction{"JNN abs", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagN), c.read16(c.PC+1), false)
	}}
	instructionSet[0x92] = instruction{"JNN @ind", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagN), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x93] = instruction{"JNN Rx", 2, func(c *CPU) {
		condJump(c, !c.getFlag(FlagN), 0, true)
	}}

	// JLS – jump if less set
	instructionSet[0x94] = instruction{"JLS #imm", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagL), c.PC+1, false)
	}}
	instructionSet[0x95] = instruction{"JLS abs", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagL), c.read16(c.PC+1), false)
	}}
	instructionSet[0x96] = instruction{"JLS @ind", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagL), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x97] = instruction{"JLS Rx", 2, func(c *CPU) {
		condJump(c, c.getFlag(FlagL), 0, true)
	}}

	// JNL – jump if less clear
	instructionSet[0x98] = instruction{"JNL #imm", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagL), c.PC+1, false)
	}}
	instructionSet[0x99] = instruction{"JNL abs", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagL), c.read16(c.PC+1), false)
	}}
	instructionSet[0x9A] = instruction{"JNL @ind", 3, func(c *CPU) {
		condJump(c, !c.getFlag(FlagL), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x9B] = instruction{"JNL Rx", 2, func(c *CPU) {
		condJump(c, !c.getFlag(FlagL), 0, true)
	}}

	// JES – jump if error set
	instructionSet[0x9C] = instruction{"JES #imm", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagE), c.PC+1, false)
	}}
	instructionSet[0x9D] = instruction{"JES abs", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagE), c.read16(c.PC+1), false)
	}}
	instructionSet[0x9E] = instruction{"JES @ind", 3, func(c *CPU) {
		condJump(c, c.getFlag(FlagE), c.read16(c.read16(c.PC+1)), false)
	}}
	instructionSet[0x9F] = instruction{"JES Rx", 2, func(c *CPU) {
		condJump(c, c.getFlag(FlagE), 0, true)
	}}

	// ── 0xA0–0xAD  Status flag operations ────────────────────────────────────

	instructionSet[0xA0] = instruction{"CSF", 1, func(c *CPU) { c.Status = 0; c.PC++ }}
	instructionSet[0xA1] = instruction{"CZF", 1, func(c *CPU) { c.setFlag(FlagZ, false); c.PC++ }}
	instructionSet[0xA2] = instruction{"SZF", 1, func(c *CPU) { c.setFlag(FlagZ, true); c.PC++ }}
	instructionSet[0xA3] = instruction{"CNF", 1, func(c *CPU) { c.setFlag(FlagN, false); c.PC++ }}
	instructionSet[0xA4] = instruction{"SNF", 1, func(c *CPU) { c.setFlag(FlagN, true); c.PC++ }}
	instructionSet[0xA5] = instruction{"CCF", 1, func(c *CPU) { c.setFlag(FlagC, false); c.PC++ }}
	instructionSet[0xA6] = instruction{"SCF", 1, func(c *CPU) { c.setFlag(FlagC, true); c.PC++ }}
	instructionSet[0xA7] = instruction{"COF", 1, func(c *CPU) { c.setFlag(FlagO, false); c.PC++ }}
	instructionSet[0xA8] = instruction{"SOF", 1, func(c *CPU) { c.setFlag(FlagO, true); c.PC++ }}
	instructionSet[0xA9] = instruction{"CLF", 1, func(c *CPU) { c.setFlag(FlagL, false); c.PC++ }}
	instructionSet[0xAA] = instruction{"SLF", 1, func(c *CPU) { c.setFlag(FlagL, true); c.PC++ }}
	instructionSet[0xAB] = instruction{"CIF", 1, func(c *CPU) { c.setFlag(FlagI, false); c.PC++ }}
	instructionSet[0xAC] = instruction{"SIF", 1, func(c *CPU) { c.setFlag(FlagI, true); c.PC++ }}
	instructionSet[0xAD] = instruction{"CEF", 1, func(c *CPU) { c.setFlag(FlagE, false); c.PC++ }}

	// ── 0xB0–0xBB  INC / INCX / DEC / DECX ──────────────────────────────────

	inc8 := func(c *CPU, src uint16, size uint16) {
		data := c.Memory[src]
		result := uint(data) + 1
		setArith8(c, data, 0x01, result)
		c.Memory[src] = uint8(result)
		c.PC += size
	}
	instructionSet[0xB0] = instruction{"INC abs", 3, func(c *CPU) { inc8(c, c.read16(c.PC+1), 3) }}
	instructionSet[0xB1] = instruction{"INC @ind", 3, func(c *CPU) { inc8(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0xB2] = instruction{"INC Rx", 2, func(c *CPU) {
		reg := c.Memory[c.PC+1]
		data := c.Registers[reg]
		result := uint(data) + 1
		setArith8(c, data, 0x01, result)
		c.Registers[reg] = uint8(result)
		c.PC += 2
	}}

	incX := func(c *CPU, src uint16, size uint16) {
		data := c.read16(src)
		result := uint(data) + 1
		setArith16(c, data, 0x0001, result)
		c.write16(src, uint16(result))
		c.PC += size
	}
	instructionSet[0xB3] = instruction{"INCX abs", 3, func(c *CPU) { incX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0xB4] = instruction{"INCX @ind", 3, func(c *CPU) { incX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0xB5] = instruction{"INCX Rx", 2, func(c *CPU) {
		reg := pairBase(c.Memory[c.PC+1])
		data := c.regPairVal(reg)
		result := uint(data) + 1
		setArith16(c, data, 0x0001, result)
		c.setRegPair(reg, uint16(result))
		c.PC += 2
	}}

	dec8 := func(c *CPU, src uint16, size uint16) {
		data := c.Memory[src]
		result := uint(data) - 1
		setArith8(c, data, 0x01, result)
		c.Memory[src] = uint8(result)
		c.PC += size
	}
	instructionSet[0xB6] = instruction{"DEC abs", 3, func(c *CPU) { dec8(c, c.read16(c.PC+1), 3) }}
	instructionSet[0xB7] = instruction{"DEC @ind", 3, func(c *CPU) { dec8(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0xB8] = instruction{"DEC Rx", 2, func(c *CPU) {
		reg := c.Memory[c.PC+1]
		data := c.Registers[reg]
		result := uint(data) - 1
		setArith8(c, data, 0x01, result)
		c.Registers[reg] = uint8(result)
		c.PC += 2
	}}

	decX := func(c *CPU, src uint16, size uint16) {
		data := c.read16(src)
		result := uint(data) - 1
		setArith16(c, data, 0x0001, result)
		c.write16(src, uint16(result))
		c.PC += size
	}
	instructionSet[0xB9] = instruction{"DECX abs", 3, func(c *CPU) { decX(c, c.read16(c.PC+1), 3) }}
	instructionSet[0xBA] = instruction{"DECX @ind", 3, func(c *CPU) { decX(c, c.read16(c.read16(c.PC+1)), 3) }}
	instructionSet[0xBB] = instruction{"DECX Rx", 2, func(c *CPU) {
		reg := pairBase(c.Memory[c.PC+1])
		data := c.regPairVal(reg)
		result := uint(data) - 1
		setArith16(c, data, 0x0001, result)
		c.setRegPair(reg, uint16(result))
		c.PC += 2
	}}
}
