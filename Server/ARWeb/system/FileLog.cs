using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Text;
using System.IO;
using System.Threading;

namespace CFileLog
{
    public class FileLog
    {
        String FileName;
        FileStream file;
        System.Threading.Mutex m = new System.Threading.Mutex(false, "LOG");

        public FileLog()
        {
        
        }

        public void Log( String str )
        {
            try
            {
                String NowTime;
                DateTime dt = DateTime.Now;
                FileName = "D:\\XRLOG\\LOG" + dt.Year + "-" + dt.Month + "-" + dt.Day + "-" + dt.Hour + ".txt";
                file = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.Write);
                NowTime = "[" + dt.Year + "-" + dt.Month + "-" + dt.Day + " " + dt.Hour + ":" + dt.Minute + ":" + dt.Second + "]";
                StreamWriter s = new StreamWriter(file);
                file.Seek(0, SeekOrigin.End);
                s.WriteLine(NowTime + str);
                s.Close();
            }
            catch(System.Exception ex)
            {
                Console.Write(ex.ToString());
            }
        }

        public void ToolLog(String str)
        {
            try
            {
                String NowTime;
                DateTime dt = DateTime.Now;
                FileName = "D:\\XRTOOLLOG\\LOG" + dt.Year + "-" + dt.Month + "-" + dt.Day + "-" + dt.Hour + ".txt";
                file = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.Write);
                NowTime = "[" + dt.Year + "-" + dt.Month + "-" + dt.Day + " " + dt.Hour + ":" + dt.Minute + ":" + dt.Second + "]";
                StreamWriter s = new StreamWriter(file);
                file.Seek(0, SeekOrigin.End);
                s.WriteLine(NowTime + str);
                s.Close();
            }
            catch(System.Exception ex)
            {
                Console.Write(ex.ToString());
            }
        }

        public void Log(String sIP, String funName, String requestStr)
        {
            try
            {
                String NowTime;
                DateTime dt = DateTime.Now;
                FileName = "D:\\XRLOG\\LOG" + dt.Year + "-" + dt.Month + "-" + dt.Day + "-" + dt.Hour + ".txt";
                file = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.Write);
                NowTime = "[" + dt.Year + "-" + dt.Month + "-" + dt.Day + " " + dt.Hour + ":" + dt.Minute + ":" + dt.Second + "]";
                StreamWriter s = new StreamWriter(file);
                file.Seek(0, SeekOrigin.End);
                s.WriteLine(NowTime + "[" + sIP + "]" + " " + funName + " " + requestStr);
                s.Close();
            }
            catch(System.Exception ex)
            {
                Console.Write(ex.ToString());
            }
        }
        public void ToolLog(String sIP, String funName, String requestStr)
        {
            try
            {
                String NowTime;
                DateTime dt = DateTime.Now;
                FileName = "D:\\XRTOOLLOG\\LOG" + dt.Year + "-" + dt.Month + "-" + dt.Day + "-" + dt.Hour + ".txt";
                file = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.Write);
                NowTime = "[" + dt.Year + "-" + dt.Month + "-" + dt.Day + " " + dt.Hour + ":" + dt.Minute + ":" + dt.Second + "]";
                StreamWriter s = new StreamWriter(file);
                file.Seek(0, SeekOrigin.End);
                s.WriteLine(NowTime + "[" + sIP + "]" + " " + funName + " " + requestStr);
                s.Close();
            }
            catch(System.Exception ex)
            {
                Console.Write(ex.ToString());
            }
        }
    }
}