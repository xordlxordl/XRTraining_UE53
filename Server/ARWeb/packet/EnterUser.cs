﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net.Json;
using System.Data.Odbc;
using System.Text;


public class EnterUser : RecvPacket
{
    public EnterUser() { }

    public override byte[] Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();

        String TID = recv.Parse(str, "tid");
        String UID = recv.Parse(str, "uid");
        String MID = recv.Parse(str, "mid");
        String Job = recv.Parse(str, "job");

        String query = "CALL spEnterUser(" + TID + "," + UID + "," + MID +  ",'"+ Job + "')";

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
