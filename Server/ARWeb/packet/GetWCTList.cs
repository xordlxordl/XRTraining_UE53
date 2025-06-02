
using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Net.Json;
using System.Web;


public class GetWCTList : RecvPacket
{
    public GetWCTList() { }
    public override byte[] Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String query = "CALL spGetWCTList()";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);

        JsonObjectCollection send = new JsonObjectCollection();
        JsonArrayCollection arr = new JsonArrayCollection("LIST");

        int i;
        int count = reader.FieldCount;
        while (reader.Read())
        {
            JsonObjectCollection item = new JsonObjectCollection();
            for (i = 0; i < count; ++i)
            {
                String col = reader.GetName(i);
                String data = reader[col].ToString();
                item.Add(new JsonStringValue(col, data));
            }
            arr.Add(item);
        }
        send.Add(arr);

        reader.Close();
        db.Close();
        return SendPacket.Send(protocol, send.ToString());
    }
}
