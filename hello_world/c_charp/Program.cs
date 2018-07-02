using System;
using System.IO;
using System.Windows.Forms;

namespace c_charp
{
    class Program
    {
        static int Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            byte[] readBytes = File.ReadAllBytes("../../../test_file.txt");
            String readStr = System.Text.Encoding.UTF8.GetString(readBytes);
            Console.WriteLine("file: " + readStr);

            System.Windows.Forms.MessageBox.Show("Message", "Caption", MessageBoxButtons.YesNo);
            return 2;
        }
    }
}
