using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Net.Json;
using System.Web;


public class Login : RecvPacket
{
    public Login(){}

    public override byte[] Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();

        String strAccount = recv.Parse(str, "account");
        String Pass = recv.Parse(str, "pass");

        String query = "CALL spLogin (" + "'" + strAccount + "'" + "," + "'" + Pass + "')";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);

        JsonObjectCollection send = new JsonObjectCollection();
        int i;
        int count = reader.FieldCount;
        while (reader.Read())
        {
            for (i = 0; i < count; ++i)
            {
                String col = reader.GetName(i);
                String data = reader[col].ToString();
                send.Add(new JsonStringValue(col, data));
            }
        }

        reader.Close();
        db.Close();
        return SendPacket.Send(protocol, send.ToString());
    }

}