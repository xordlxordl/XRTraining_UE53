
using System;

using System.Data.Odbc;


public class LOGMOTION : RecvLog
{
    public LOGMOTION() { }

    public override void Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String TID = recv.Parse(str, "tid");
        
        String motion = recv.Parse(str, "motion");

        String query = "CALL log_motion(  " + TID + "," + "'" + motion + "'"+")";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);

        db.Close();

    }
}
