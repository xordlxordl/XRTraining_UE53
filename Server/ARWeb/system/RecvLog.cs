
using System;

public abstract class RecvLog
{
    public RecvLog()
    {

    }

    //public abstract byte[] Recv( String str );
    public abstract void Recv(String str, int protocol);

}
