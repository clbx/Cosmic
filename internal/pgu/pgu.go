package pgu

// VRAM address range in Cosmic memory map.
const (
	VRAMStart = 0x8000
	VRAMEnd   = 0xBFFF
	VRAMSize  = VRAMEnd - VRAMStart + 1
)

// Config defines how video memory is interpreted and rendered.
type Config struct {
	Width      int    // pixels wide
	Height     int    // pixels tall
	ColorDepth int    // bytes per pixel (1, 2, or 4)
	Name       string // e.g. "default", "color", "64x64-8x8_TrueColor"
}

// DefaultConfig matches the original "default" PGU configuration.
var DefaultConfig = Config{
	Width:      75,
	Height:     50,
	ColorDepth: 1,
	Name:       "default",
}

// PGU converts video memory into an RGBA pixel buffer.
type PGU struct {
	Config Config
}

// New returns a PGU with the given config.
func New(cfg Config) *PGU {
	return &PGU{Config: cfg}
}

// Render reads VRAM from the full memory array and returns an RGBA byte slice
// suitable for encoding as a PNG and sending to the frontend.
func (p *PGU) Render(memory []byte) []byte {
	w := p.Config.Width
	h := p.Config.Height
	out := make([]byte, w*h*4) // RGBA

	// TODO: implement color depth modes and palette lookup
	// For now: single-byte grayscale (value → R, G, B with A=255)
	for i := 0; i < w*h && i < VRAMSize; i++ {
		v := memory[VRAMStart+i]
		base := i * 4
		out[base+0] = v   // R
		out[base+1] = v   // G
		out[base+2] = v   // B
		out[base+3] = 255 // A
	}

	return out
}
