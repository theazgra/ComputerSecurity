using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StartupProgrma
{
    class Program
    {
        static void Main(string[] args)
        {
            using (StreamWriter writer = new StreamWriter(@"C:\Users\mor0146\Documents\StartupLog.txt",true))
            {
                writer.WriteLine("Program started at {0}.", DateTime.Now);
            }
        }
    }
}
