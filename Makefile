.PHONY: all build frontend run dev test clean

all: build

## Build the frontend then compile the Go binary
build: frontend
	go build -o cosmic ./cmd/cosmic/

## Build the React/Vite frontend into frontend/dist/
frontend:
	cd frontend && npm install --silent && npm run build

## Build and run the server (serves frontend/dist/)
run: build
	./cosmic

## Development mode: Vite dev server (port 5173) + Go backend (port 8080)
## Access the app at http://localhost:5173
dev:
	@cd frontend && npm install --silent && npm run dev &
	@go run ./cmd/cosmic/

## Run all Go tests
test:
	go test ./...

## Remove build artifacts
clean:
	rm -f cosmic
	rm -rf frontend/dist
