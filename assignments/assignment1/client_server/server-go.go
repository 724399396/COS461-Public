/*****************************************************************************
 * server-go.go
 * Name:
 * NetId:
 *****************************************************************************/

package main

import (
	"io"
	"log"
	"net"
	"os"
)

const RECV_BUFFER_SIZE = 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 */
func server(server_port string) {
	ln, err := net.Listen("tcp", ":"+server_port)
	if err != nil {
		panic(err)
	}
	for {
		conn, _ := ln.Accept()

		buf := make([]byte, RECV_BUFFER_SIZE)
		defer conn.Close()
		for {
      n, err := conn.Read(buf)
			buf = buf[:n]
			if err != nil {
				if err == io.EOF {
					break
				}
				panic(err)
			}
      os.Stdout.Write(buf)
		}
	}
}

// Main parses command-line arguments and calls server function
func main() {
	if len(os.Args) != 2 {
		log.Fatal("Usage: ./server-go [server port]")
	}
	server_port := os.Args[1]
	server(server_port)
}
