from http.server import BaseHTTPRequestHandler, HTTPServer

class CredHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode('utf-8')
        print("Received credentials:", post_data)
        self.send_response(200)
        self.end_headers()

server_address = ('', 8080)
httpd = HTTPServer(server_address, CredHandler)
print('Starting server...')
httpd.serve_forever()
