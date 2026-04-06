package server

import (
	"bufio"
	"bytes"
	"crypto/sha1"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"image"
	"image/png"
	"io"
	"log"
	"net"
	"net/http"
	"time"
)

// wsMessage is the envelope pushed to all connected clients.
type wsMessage struct {
	Type    string `json:"type"`
	Payload any    `json:"payload"`
}

// inboundMsg is the envelope received from clients (e.g. key events).
type inboundMsg struct {
	Type string `json:"type"`
	Key  int    `json:"key"`
}

// upgradeWS performs the WebSocket handshake and returns the hijacked connection.
// For pre-hijack errors it writes an HTTP error response itself; for post-hijack
// errors it closes the connection and returns a non-nil error (caller must not
// touch w after this returns).
func upgradeWS(w http.ResponseWriter, r *http.Request) (net.Conn, *bufio.Reader, error) {
	key := r.Header.Get("Sec-WebSocket-Key")
	if key == "" {
		http.Error(w, "missing Sec-WebSocket-Key", http.StatusBadRequest)
		return nil, nil, fmt.Errorf("ws: missing Sec-WebSocket-Key")
	}

	hj, ok := w.(http.Hijacker)
	if !ok {
		http.Error(w, "hijacking not supported", http.StatusInternalServerError)
		return nil, nil, fmt.Errorf("ws: hijacking not supported")
	}

	conn, buf, err := hj.Hijack()
	if err != nil {
		return nil, nil, fmt.Errorf("ws: hijack: %w", err)
	}

	// Compute Sec-WebSocket-Accept
	h := sha1.New()
	h.Write([]byte(key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"))
	accept := base64.StdEncoding.EncodeToString(h.Sum(nil))

	_, err = fmt.Fprintf(buf,
		"HTTP/1.1 101 Switching Protocols\r\n"+
			"Upgrade: websocket\r\n"+
			"Connection: Upgrade\r\n"+
			"Sec-WebSocket-Accept: %s\r\n\r\n", accept)
	if err == nil {
		err = buf.Flush()
	}
	if err != nil {
		conn.Close()
		return nil, nil, fmt.Errorf("ws: write 101: %w", err)
	}

	return conn, buf.Reader, nil
}

// writeWSFrame sends an unmasked WebSocket frame (server → client).
// opcode: 0x1 = text, 0x2 = binary, 0x8 = close, 0xA = pong.
func writeWSFrame(conn net.Conn, opcode byte, payload []byte) error {
	hdr := make([]byte, 0, 10)
	hdr = append(hdr, 0x80|opcode) // FIN bit set

	n := len(payload)
	switch {
	case n < 126:
		hdr = append(hdr, byte(n))
	case n < 65536:
		hdr = append(hdr, 126, byte(n>>8), byte(n))
	default:
		hdr = append(hdr, 127,
			0, 0, 0, 0,
			byte(n>>24), byte(n>>16), byte(n>>8), byte(n))
	}

	if _, err := conn.Write(hdr); err != nil {
		return err
	}
	if len(payload) > 0 {
		_, err := conn.Write(payload)
		return err
	}
	return nil
}

// readWSFrame reads one masked WebSocket frame (client → server).
// Returns opcode and unmasked payload.
func readWSFrame(r *bufio.Reader) (opcode byte, payload []byte, err error) {
	hdr := make([]byte, 2)
	if _, err = io.ReadFull(r, hdr); err != nil {
		return
	}

	opcode = hdr[0] & 0x0F
	masked := hdr[1]&0x80 != 0
	payLen := int(hdr[1] & 0x7F)

	switch payLen {
	case 126:
		ext := make([]byte, 2)
		if _, err = io.ReadFull(r, ext); err != nil {
			return
		}
		payLen = int(ext[0])<<8 | int(ext[1])
	case 127:
		ext := make([]byte, 8)
		if _, err = io.ReadFull(r, ext); err != nil {
			return
		}
		// Only lower 32 bits are realistic for our use case
		payLen = int(ext[4])<<24 | int(ext[5])<<16 | int(ext[6])<<8 | int(ext[7])
	}

	var mask [4]byte
	if masked {
		if _, err = io.ReadFull(r, mask[:]); err != nil {
			return
		}
	}

	payload = make([]byte, payLen)
	if _, err = io.ReadFull(r, payload); err != nil {
		return
	}

	if masked {
		for i := range payload {
			payload[i] ^= mask[i%4]
		}
	}
	return
}

// encodePNGFrame wraps an RGBA byte slice in an image.RGBA and PNG-encodes it.
func encodePNGFrame(rgba []byte, w, h int) ([]byte, error) {
	img := &image.RGBA{
		Pix:    rgba,
		Stride: w * 4,
		Rect:   image.Rect(0, 0, w, h),
	}
	var buf bytes.Buffer
	if err := png.Encode(&buf, img); err != nil {
		return nil, err
	}
	return buf.Bytes(), nil
}

// handleWS upgrades the connection and streams CPU state + PGU frame at 60 fps.
// It also accepts inbound JSON messages; type "key" writes a keycode to Memory[0xC400].
func (s *Server) handleWS(w http.ResponseWriter, r *http.Request) {
	conn, reader, err := upgradeWS(w, r)
	if err != nil {
		log.Println("WebSocket upgrade error:", err)
		return
	}
	defer conn.Close()

	// Goroutine: read inbound frames from client, forward key events.
	inbound := make(chan inboundMsg, 8)
	connClosed := make(chan struct{})
	go func() {
		defer close(connClosed)
		for {
			opcode, payload, err := readWSFrame(reader)
			if err != nil {
				return
			}
			switch opcode {
			case 0x8: // Close
				return
			case 0x9: // Ping — reply with Pong
				writeWSFrame(conn, 0xA, payload) //nolint
			case 0x1, 0x2: // Text or Binary — parse as JSON command
				var msg inboundMsg
				if json.Unmarshal(payload, &msg) == nil {
					select {
					case inbound <- msg:
					default: // drop if buffer full
					}
				}
			}
		}
	}()

	ticker := time.NewTicker(time.Second / 60)
	defer ticker.Stop()

	for {
		select {
		case <-connClosed:
			return

		case msg := <-inbound:
			if msg.Type == "key" {
				s.mu.Lock()
				s.cpu.Memory[0xC400] = byte(msg.Key)
				s.mu.Unlock()
			}

		case <-ticker.C:
			s.mu.Lock()
			snap := s.cpu.Snapshot()
			running := s.running
			rgba := s.pgu.Render(s.cpu.Memory[:])
			s.mu.Unlock()

			frame, err := encodePNGFrame(rgba, s.pgu.Config.Width, s.pgu.Config.Height)
			if err != nil {
				log.Println("ws: PGU encode error:", err)
				continue
			}

			payload, err := json.Marshal(wsMessage{
				Type: "state",
				Payload: map[string]any{
					"state":   snap,
					"running": running,
					"frame":   base64.StdEncoding.EncodeToString(frame),
				},
			})
			if err != nil {
				continue
			}

			if err := writeWSFrame(conn, 0x1, payload); err != nil {
				return
			}
		}
	}
}
