
using System;

using System.Net.Json;
using System.Data.Odbc;



public class DefaultLog : RecvLog
{
    public DefaultLog() { }

    public override void Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String TID = recv.Parse(str, "tid");
        String UID = recv.Parse(str, "uid");


    }
}
