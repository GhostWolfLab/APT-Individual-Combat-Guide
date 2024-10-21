from http.server import BaseHTTPRequestHandler, HTTPServer

class HTTPTunnelHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # 读取请求中的数据并打印
        data = self.path[1:]
        print(f"Received data: {data}")

        # 发送响应
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(b"Data received")

def run(server_class=HTTPServer, handler_class=HTTPTunnelHandler, port=80):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting httpd on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()
