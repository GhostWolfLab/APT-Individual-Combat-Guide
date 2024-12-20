using System;
using System.EnterpriseServices;
using System.Runtime.InteropServices;
using System.Windows.Forms;
namespace regsvcser
{
    public class Bypass : ServicedComponent
    {
        public Bypass()
        {
            Console.WriteLine("I wanted you!");
        }

        [ComRegisterFunction]
        public static void RegisterClass(string key)
        {
            MessageBox.Show("Registration successful!", "Notification", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        [ComUnregisterFunction]
        public static void UnRegisterClass(string key)
        {
            MessageBox.Show("Unregistration successful!", "Notification", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
