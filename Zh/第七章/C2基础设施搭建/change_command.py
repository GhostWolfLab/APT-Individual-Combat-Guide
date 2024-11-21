import requests

# 服务器地址（端点）
url = "http://localhost:8080/update_command"

# 新命令
new_command = "ls -la"  # 例如，列出当前目录的所有文件

# 构造请求数据
data = {
    "command": new_command
}

# 发送HTTP POST请求
response = requests.post(url, json=data)

# 检查响应
if response.status_code == 200:
    print("Command updated successfully!")
else:
    print(f"Failed to update command. Status code: {response.status_code}")
