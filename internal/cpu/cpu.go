package cpu

// CPU represents the Cosmic processor state.
type CPU struct {
	// General purpose registers: A, B, C, D, E, F, G, H
	Registers [8]uint8

	// 16-bit program counter
	PC uint16

	// 16-bit stack pointer (initialized at 0xC399)
	SP uint16

	// Status register: Z N C O L I E X
	Status uint8

	// 64K addressable memory
	Memory [65536]uint8

	// Halted indicates the CPU has executed HCF
	Halted bool
}

// New creates and returns an initialized CPU.
func New() *CPU {
	c := &CPU{
		SP: 0xC399,
	}
	c.buildInstructionSet()
	return c
}

// Reset clears registers and resets PC to the reset vector.
func (c *CPU) Reset() {
	c.Registers = [8]uint8{}
	c.Status = 0
	c.Halted = false
	c.SP = 0xC399
	lo := uint16(c.Memory[0xFFF0])
	hi := uint16(c.Memory[0xFFF1])
	c.PC = (hi << 8) | lo
}

func (c *CPU) ResetMemory() {
	for i := 0; i < len(c.Memory); i++ {
		c.Memory[i] = 0
	}
}

// LoadROM copies bytes into memory starting at the given address.
func (c *CPU) LoadROM(data []byte, origin uint16) {
	copy(c.Memory[origin:], data)
}

// Read returns the byte at the given address.
func (c *CPU) Read(addr uint16) uint8 {
	return c.Memory[addr]
}

// Write stores a byte at the given address.
func (c *CPU) Write(addr uint16, val uint8) {
	c.Memory[addr] = val
}

// Cycle executes a single fetch-decode-execute cycle.
// Returns false if the CPU is halted.
func (c *CPU) Cycle() bool {
	if c.Halted {
		return false
	}
	opcode := c.Read(c.PC)
	inst := instructionSet[opcode]
	inst.execute(c)
	return !c.Halted
}

// RegisterNames maps register index to its name.
var RegisterNames = [8]string{"A", "B", "C", "D", "E", "F", "G", "H"}

// State is a snapshot of the CPU suitable for sending to the frontend.
type State struct {
	Registers [8]uint8 `json:"registers"`
	PC        uint16   `json:"pc"`
	SP        uint16   `json:"sp"`
	Status    uint8    `json:"status"`
	Halted    bool     `json:"halted"`
	Flags     Flags    `json:"flags"`
}

// Snapshot returns the current CPU state as a State struct.
func (c *CPU) Snapshot() State {
	return State{
		Registers: c.Registers,
		PC:        c.PC,
		SP:        c.SP,
		Status:    c.Status,
		Halted:    c.Halted,
		Flags:     c.decodeFlags(),
	}
}
