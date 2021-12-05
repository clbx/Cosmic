package main

import (
	"fmt"

	cosmic "github.com/clbx/cosmic/cosproc"
)

func main() {
	fmt.Printf("hello world")
	c := cosmic.NewCosproc()

	cosmic.Cycle(c)

	fmt.Printf("%X", cosmic.MemRead(&c.Mem, 0x0000))

}
