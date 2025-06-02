
using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Net.Json;
using System.Data.Odbc;
using System.Text;


public class CheckFile : RecvPacket
{
    public CheckFile() { }

    public override byte[] Recv(String str, int protocol)
    {
        JsonParser recv = new JsonParser();
        String filename = recv.Parse(str, "filename");
        String filetype = recv.Parse(str, "filetype");

        String query = "CALL spCheckFile( '" + filename + "'," + filetype + ")";

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query);

        JsonObjectCollection send = new JsonObjectCollection();

        int nResult = 0;
        String strFilePullPath = String.Empty;

        while (reader.Read())
        {
            String resultkey = "RESULT";
            String resultvalue = reader[resultkey].ToString();
            nResult = Convert.ToInt32(resultvalue);

            if (nResult == 1)
            {
                String col = "fullpath";
                String data = reader[col].ToString();
                strFilePullPath = data;

            }

        }

        reader.Close();
        db.Close();

        int RealFile = 0;

        if (nResult == 1)
        {
            if (File.Exists(strFilePullPath))
            {
                RealFile = 1;
            }
        }

        send.Add(new JsonStringValue("filename", filename));
        send.Add(new JsonStringValue("filetype", filetype));
        send.Add(new JsonStringValue("bHaveFile", RealFile.ToString() ));



        return SendPacket.Send(protocol, send.ToString());
    }
}
