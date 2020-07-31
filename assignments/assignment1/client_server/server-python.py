###############################################################################
# server-python.py
# Name:
# NetId:
###############################################################################

import sys
import socket

RECV_BUFFER_SIZE = 2048
QUEUE_LENGTH = 10

def server(server_port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('', server_port))
    s.listen(QUEUE_LENGTH)
    while 1:
        conn, addr = s.accept()
        while 1:
            data = conn.recv(RECV_BUFFER_SIZE)
            if not data: 
                break
            sys.stdout.write(data)
            sys.stdout.flush()
        conn.close()


def main():
    """Parse command-line argument and call server function """
    if len(sys.argv) != 2:
        sys.exit("Usage: python server-python.py [Server Port]")
    server_port = int(sys.argv[1])
    server(server_port)

if __name__ == "__main__":
    main()
