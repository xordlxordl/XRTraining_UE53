
using System;

using System.Net.Json;
using System.Data.Odbc;



public class LOGFIRE : RecvLog
{
    public LOGFIRE(){}

    public override void Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String TID = recv.Parse(str, "tid");
        String UID = recv.Parse(str, "uid");

        String query = "CALL log_fire(  "+ TID + "," + UID + ")";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);
        
        db.Close();
        
    }
}
