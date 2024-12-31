$filePath = "C:\Users\Administrator\Desktop\test\payload\Addins.Store"

$existingContent = Get-Content -Path $filePath -Encoding Byte
$modifiedContent = [byte[]](0) * 12 + $existingContent
$modifiedContent | Set-Content -Path $filePath -Encoding Byte -NoNewline
