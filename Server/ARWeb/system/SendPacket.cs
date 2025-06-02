using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net.Json;
using System.Text;
using CFileLog;
public class SendPacket
{
    public SendPacket()
    {
    
    }

    public static byte[] Send( int protocol, String str )
    {

        return SendByte(protocol, str);
        /*
        String JsonString;
        JsonObjectCollection json = new JsonObjectCollection();
        json.Add(new JsonStringValue("protocol", Convert.ToString( protocol ) ) );
        json.Add(new JsonStringValue("result", str));
        JsonString = json.ToString();
        byte[] strByte = Encoding.UTF8.GetBytes(JsonString);

        

        return strByte;
        */
        /*
        String JsonString;
        JsonObjectCollection json = new JsonObjectCollection();
        json.Add( new JsonStringValue( "protocol", Convert.ToString( protocol ) ) );
        json.Add( new JsonStringValue( "result", str ) );
        JsonString = json.ToString();


        Int32[] UnZiplen = new int[1];

        Int32[] zlen = new int[1];
        int ziplen = 0;

        byte[] bStrByteA = Encoding.UTF8.GetBytes( JsonString );   // string -> byte
        byte[] Out = lzf.ZIP(bStrByteA, ref ziplen);

        UnZiplen[0] = bStrByteA.Length;
        zlen[0] = ziplen;

        int PaketSize = ziplen + 8;
        byte[] Packet = new byte[ PaketSize ];

        Buffer.BlockCopy( UnZiplen, 0, Packet, 0, 4 );  //압축패킷헤더 압축안한 사이즈
        Buffer.BlockCopy( zlen, 0, Packet, 4, 4 );      //압축패킷헤더 압축한 사이즈
        Buffer.BlockCopy( Out, 0, Packet, 8, ziplen );  //압축된 패킷 몸통

        FileLog log = new FileLog();
        log.Log("0.0.0.0", "Send", "UnZiplen=" + bStrByteA.Length.ToString());
        log.Log("0.0.0.0", "Send", "ziplen=" + ziplen.ToString());
        return Packet;
        */
    }

    public static byte[] SendByte(int protocol, String str)
    {
        String JsonString;
        JsonObjectCollection json = new JsonObjectCollection();
        json.Add(new JsonStringValue("protocol", Convert.ToString(protocol)));
        json.Add(new JsonStringValue("result", str));
        JsonString = json.ToString();
        //byte[] strByte = Encoding.UTF8.GetBytes(JsonString);
        byte[] strByte = Encoding.UTF8.GetBytes(JsonString);
        return strByte;
    }

    
}
