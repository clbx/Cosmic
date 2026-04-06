package main

import (
	"fmt"
	"log"
	"net/http"

	"github.com/clbx/cosmic/internal/server"
)

const defaultPort = "8080"

func main() {
	srv := server.New()

	addr := "localhost:" + defaultPort
	fmt.Printf("Cosmic running at http://%s\n", addr)

	log.Fatal(http.ListenAndServe(addr, srv))
}
