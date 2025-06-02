
using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.IO;
using System.Linq;
using System.Net.Json;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ARWeb
{
    public partial class upload_png : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            HttpFileCollection files = Request.Files;

            JsonArrayCollection arr = new JsonArrayCollection("LIST");

            int i;
            int filecount = files.Count;
            for (i = 0; i < filecount; i++)
            {
                HttpPostedFile file = files[i];
                int FileSize = file.ContentLength;
                if (file == null)
                {
                    continue;
                }

                String RecvName = file.FileName;

                String dir = "D://Military//PngFiles//";

                String filename = RecvName;

                String NewName = filename;
                String FullName = dir + NewName;
                /*
                if (File.Exists(FullName))
                {
                    //중복 파일 덮어 버리는 걸로
                    //NewName = DuplicateFile(dir, filename );
                    FullName = dir + NewName;
                }
                */



                try
                {
                    file.SaveAs(FullName);
                }
                catch (Exception ex)
                {
                    Response.Write(ex.Message);
                    return;
                }


                String query = "CALL spAddPNG(" + "'" + NewName + "'" + "," + "'" + FullName + "')";



                DB db = new DB();
                db.Exec(query);

                JsonObjectCollection send = new JsonObjectCollection();


                send.Add(new JsonStringValue("filename", FullName));
                send.Add(new JsonStringValue("FileSize", FileSize.ToString()));


                db.Close();

                arr.Add(send);
            }

            Response.Write(arr.ToString());
        }
    }
}