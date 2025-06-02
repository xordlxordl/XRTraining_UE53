using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net;
using System.Text;
using System.IO;

public class MyURL
{
    public MyURL()
    {
    
    }
    public string SendPOST( string url, string data )
    {
        StringBuilder str = new StringBuilder();
        str.Append( data );

        Encoding encoding = Encoding.UTF8;
        byte[] result = encoding.GetBytes( str.ToString() );

        string strURL = url;

        HttpWebRequest req = ( HttpWebRequest )WebRequest.Create( strURL );
        
        req.Method = "POST";
        req.ContentType = "text/plain";
        req.ContentLength = result.Length;


        Stream StreamData = req.GetRequestStream();
        StreamData.Write( result, 0, result.Length );
        StreamData.Close();

        HttpWebResponse Respone = (HttpWebResponse)req.GetResponse();

        // Response의 결과를 스트림을 생성합니다.
        Stream StreamRespone = Respone.GetResponseStream();
        
        StreamReader ReaderRespone = new StreamReader( StreamRespone, Encoding.UTF8 );
        
        return ReaderRespone.ReadToEnd();
    }

    public string SendWWW(string url, string data)
    {
        StringBuilder str = new StringBuilder();
        str.Append(data);

        Encoding encoding = Encoding.UTF8;
        byte[] result = encoding.GetBytes(str.ToString());

        string strURL = url;

        HttpWebRequest req = (HttpWebRequest)WebRequest.Create(strURL);

        req.Method = "POST";
        req.ContentType = "application/x-www-form-urlencoded";
        req.ContentLength = result.Length;

        Stream StreamData = req.GetRequestStream();
        StreamData.Write(result, 0, result.Length);
        StreamData.Close();

        HttpWebResponse Respone = (HttpWebResponse)req.GetResponse();

        // Response의 결과를 스트림을 생성합니다.
        Stream StreamRespone = Respone.GetResponseStream();

        StreamReader ReaderRespone = new StreamReader(StreamRespone, Encoding.UTF8);

        return ReaderRespone.ReadToEnd();
    }

    public string SendPostJSON(string url, string data)
    {
        StringBuilder str = new StringBuilder();
        str.Append(data);

        Encoding encoding = Encoding.UTF8;
        byte[] result = encoding.GetBytes(str.ToString());


        string strURL = url;

        HttpWebRequest req = (HttpWebRequest)WebRequest.Create(strURL);

        req.Method = "POST";
        req.ContentType = "application/json";
        req.ContentLength = result.Length;


        Stream StreamData = req.GetRequestStream();
        StreamData.Write(result, 0, result.Length);
        StreamData.Close();

        HttpWebResponse Respone = (HttpWebResponse)req.GetResponse();

        // Response의 결과를 스트림을 생성합니다.
        Stream StreamRespone = Respone.GetResponseStream();

        StreamReader ReaderRespone = new StreamReader(StreamRespone, Encoding.UTF8);
        //StreamReader ReaderRespone = new StreamReader( StreamRespone, Encoding.Default );
        // 생성한 스트림으로부터 string으로 변환합니다.
        return ReaderRespone.ReadToEnd();
    }
    public string Send( string url )
    {

        Encoding encoding = Encoding.UTF8;
        byte[] result = encoding.GetBytes(url.ToString());

        HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);

        req.Method = "POST";
        req.ContentType = "application/x-www-form-urlencoded";
        req.ContentLength = result.Length;


        Stream StreamData = req.GetRequestStream();
        StreamData.Write(result, 0, result.Length);
        StreamData.Close();

        HttpWebResponse Respone = (HttpWebResponse)req.GetResponse();


        Stream StreamRespone = Respone.GetResponseStream();

        StreamReader ReaderRespone = new StreamReader(StreamRespone, Encoding.UTF8);

        return ReaderRespone.ReadToEnd();
    }
}
