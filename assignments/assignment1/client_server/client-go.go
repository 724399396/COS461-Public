/*****************************************************************************
 * client-go.go
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

const SEND_BUFFER_SIZE = 2048

/* client()
 * Open socket and send message from stdin.
 */
func client(server_ip string, server_port string) {
	buf := make([]byte, SEND_BUFFER_SIZE)
	conn, err := net.Dial("tcp", server_ip+":"+server_port)
	if err != nil {
		panic(err)
	}
	defer conn.Close()
	for {
		n, err := os.Stdin.Read(buf)
		buf = buf[:n]
		if err != nil {
			if err == io.EOF {
				break
			}
			panic(err)
		}
		conn.Write(buf)
	}
}

// Main parses command-line arguments and calls client function
func main() {
	if len(os.Args) != 3 {
		log.Fatal("Usage: ./client-go [server IP] [server port] < [message file]")
	}
	server_ip := os.Args[1]
	server_port := os.Args[2]
	client(server_ip, server_port)
}
