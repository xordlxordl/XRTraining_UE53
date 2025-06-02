using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;


public class Default : RecvPacket
{
    public Default() {}
    public override byte[] Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        return SendPacket.Send(protocol, str);
    }
}
