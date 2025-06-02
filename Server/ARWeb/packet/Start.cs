
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net.Json;
using System.Data.Odbc;
using System.Text;


public class Start : RecvPacket
{
    public Start() { }

    public override byte[] Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String TName = recv.Parse(str, "tname");

        String query = "CALL spStart( '"+ TName + "')";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);

        JsonObjectCollection send = new JsonObjectCollection();

        int i;
        int count = reader.FieldCount;
        while( reader.Read() )
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
