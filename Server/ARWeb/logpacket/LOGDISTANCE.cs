
using System;

using System.Net.Json;
using System.Data.Odbc;



public class LOGDISTANCE : RecvLog
{
    public LOGDISTANCE() { }

    public override void Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String TID = recv.Parse(str, "tid");
        String UID = recv.Parse(str, "uid");
        String Dist = recv.Parse(str, "dist");

        String query = "CALL log_distance(  " + TID + "," + UID + "," + Dist + ")";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);

        db.Close();

    }
}
