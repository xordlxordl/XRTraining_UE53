﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net.Json;
using System.Data.Odbc;
using System.Text;
using System.Security.Cryptography;


public class TrainingDetail : RecvPacket
{
    public TrainingDetail() { }

    public override byte[] Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String TID = recv.Parse(str, "tid");
        String UID = recv.Parse(str, "uid");

        String query = "CALL spGetUserLogDetail(" + TID + "," + UID + ")";


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
