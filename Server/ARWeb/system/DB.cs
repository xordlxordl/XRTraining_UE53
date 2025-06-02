using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using System.Data.Odbc;
using CFileLog;
using System.Configuration;

public class DB
{
    OdbcConnection Con;
    public DB()
    {
    
    }
    ~DB()
    {
    
    }
    public void Close()
    {
        Con.Close();
    }
    public OdbcDataReader Exec( String query )
    {
        //string ConnectString = ConfigurationSettings.AppSettings["ConnectString"];
        //string ConnectString = "Provider=SQLOLEDB;Data Source=192.168.0.64,3306;Initial Catalog=xrts;User ID=root;Password=interact";
        //string ConnectString = "Provider=MySQLProv;Data Source=192.168.0.64:3306;Initial Catalog=xrts;User ID=root;Password=interact";
        string ConnectString = "DRIVER={MARIADB ODBC 3.1 Driver}; SERVER=192.168.0.20; PORT=3306; DATABASE=arts; UID=root; Pwd=interact;";

        //string ConnectString = ConfigurationSettings.AppSettings["ConnectString"];
        //string ConnectString = System.Configuration.ConfigurationManager.AppSettings["ConnectString"];

        Con = new OdbcConnection( ConnectString );
        try
        {
            Con.Open();
            OdbcCommand Cmd = new OdbcCommand();
            Cmd.Connection = Con;
            Cmd.CommandText = query;
            return Cmd.ExecuteReader();
        }
        catch( System.Exception ex )
        {
            FileLog log = new FileLog();
            log.Log( ex.ToString() );
            return null;
        }
    }
    public int Query( String query )
    {

        //string ConnectString = ConfigurationSettings.AppSettings["ConnectString"];
        string ConnectString = "DRIVER={MARIADB ODBC 3.1 Driver}; SERVER=192.168.0.64; PORT=3306; DATABASE=arts; UID=root; Pwd=interact;";

        Con = new OdbcConnection(ConnectString);
        try
        {
            Con.Open();

            OdbcCommand Cmd = new OdbcCommand();
            Cmd.Connection = Con;
            Cmd.CommandText = query;

            return Cmd.ExecuteNonQuery();
                
        }
        catch( System.Exception ex )
        {
            FileLog log = new FileLog();
            log.Log(ex.ToString());
            return -1;
        }
    }

}
