import ssl
import socket

def send_data(server, port, data):
    context = ssl.create_default_context()
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE

    with socket.create_connection((server, port)) as sock:
        with context.wrap_socket(sock, server_hostname=server) as ssock:
            ssock.sendall(data)

server = 'localhost'
port = 8443
data = b'Hi,I am snowwolf'
send_data(server, port, data)
