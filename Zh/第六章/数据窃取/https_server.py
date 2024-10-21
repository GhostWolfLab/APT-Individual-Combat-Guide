import socket
import ssl

def start_server():
    context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
    context.load_cert_chain(certfile="server.crt", keyfile="server.key")

    bindsocket = socket.socket()
    bindsocket.bind(('0.0.0.0', 8443))
    bindsocket.listen(5)

    while True:
        newsocket, fromaddr = bindsocket.accept()
        conn = context.wrap_socket(newsocket, server_side=True)
        try:
            data = conn.recv(1024)
            print(f"Received data: {data.decode('utf-8')}")
            conn.sendall(b"Data received")
        finally:
            conn.shutdown(socket.SHUT_RDWR)
            conn.close()

if __name__ == "__main__":
    start_server()
