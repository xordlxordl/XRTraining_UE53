
using System;

using System.Net.Json;
using System.Data.Odbc;



public class LOGTRHMD : RecvLog
{
    public LOGTRHMD() { }

    public override void Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String TID = recv.Parse(str, "tid");
        String UID = recv.Parse(str, "uid");
        String x = recv.Parse(str, "x");
        String y = recv.Parse(str, "y");
        String z = recv.Parse(str, "z");
        String p = recv.Parse(str, "p");
        String r = recv.Parse(str, "r");
        String w = recv.Parse(str, "w");

        String query = "CALL log_tr_hmd(  " + TID + "," + UID + "," + x + "," + y + "," + z + "," + p + "," + r + "," + w +")";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);

        db.Close();

    }
}
