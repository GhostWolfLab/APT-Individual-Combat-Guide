using System;
using System.Windows.Forms;

namespace MaliciousNamespace
{
    public class MaliciousClass
    {
        public static void ShowMessage(string message)
        {
            MessageBox.Show(message, "Malicious Code", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
    }
}
