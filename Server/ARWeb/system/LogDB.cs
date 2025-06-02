using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Data.OleDb;
using CFileLog;
using System.Configuration;

public class LogDB
{
    OleDbConnection Con;
    public LogDB()
    {

    }
    ~LogDB()
    {

    }
    public void Close()
    {
        Con.Close();
    }
    public OleDbDataReader Exec(String query)
    {
        //string ConnectString = ConfigurationSettings.AppSettings["ConnectString"];
		string ConnectString = "Provider=SQLOLEDB;Data Source=127.0.0.1;Initial Catalog=we;User ID=nterfun;Password=fun2013";
        
        Con = new OleDbConnection(ConnectString);
        try
        {
            Con.Open();
            OleDbCommand Cmd = new OleDbCommand();
            Cmd.Connection = Con;
            Cmd.CommandText = query;
            return Cmd.ExecuteReader();
        }
        catch(System.Exception ex)
        {
            FileLog log = new FileLog();
            log.Log(ex.ToString());
            return null;
        }
    }
    public int Query(String query)
    {
        //string ConnectString = ConfigurationSettings.AppSettings["ConnectString"];
		string ConnectString = "Provider=SQLOLEDB;Data Source=127.0.0.1;Initial Catalog=we;User ID=nterfun;Password=fun2013";

        Con = new OleDbConnection(ConnectString);
        try
        {
            Con.Open();

            OleDbCommand Cmd = new OleDbCommand();
            Cmd.Connection = Con;
            Cmd.CommandText = query;

            return Cmd.ExecuteNonQuery();

        }
        catch(System.Exception ex)
        {
            FileLog log = new FileLog();
            log.Log(ex.ToString());
            return -1;
        }
    }

}
