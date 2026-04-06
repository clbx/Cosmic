package cpu

// Status register bit positions.
const (
	FlagZ uint8 = 1 << 0 // Zero
	FlagN uint8 = 1 << 1 // Negative
	FlagC uint8 = 1 << 2 // Carry
	FlagO uint8 = 1 << 3 // Overflow
	FlagL uint8 = 1 << 4 // Less (signed comparison)
	FlagI uint8 = 1 << 5 // Interrupt disable
	FlagE uint8 = 1 << 6 // Error (divide by zero)
)

// Flags is a human-readable breakdown of the status register.
type Flags struct {
	Zero      bool `json:"z"`
	Negative  bool `json:"n"`
	Carry     bool `json:"c"`
	Overflow  bool `json:"o"`
	Less      bool `json:"l"`
	Interrupt bool `json:"i"`
	Error     bool `json:"e"`
}

func (c *CPU) decodeFlags() Flags {
	return Flags{
		Zero:      c.Status&FlagZ != 0,
		Negative:  c.Status&FlagN != 0,
		Carry:     c.Status&FlagC != 0,
		Overflow:  c.Status&FlagO != 0,
		Less:      c.Status&FlagL != 0,
		Interrupt: c.Status&FlagI != 0,
		Error:     c.Status&FlagE != 0,
	}
}

func (c *CPU) setFlag(flag uint8, val bool) {
	if val {
		c.Status |= flag
	} else {
		c.Status &^= flag
	}
}

func (c *CPU) getFlag(flag uint8) bool {
	return c.Status&flag != 0
}
