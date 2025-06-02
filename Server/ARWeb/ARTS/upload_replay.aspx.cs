using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Data.Odbc;
using System.IO;
using System.Linq;
using System.Net.Json;
using System.Security.Principal;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using static System.Net.Mime.MediaTypeNames;

namespace ARWeb.ARTS
{
    public partial class upload_replay : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            JsonObjectCollection send = new JsonObjectCollection();

            //HttpFileCollection files = Request.Files[0];
            HttpPostedFile file = Request.Files[0];
            //Request.Headers.

            NameValueCollection coll;
            coll = Request.Headers;

            String TID = coll.Get("TID");

            if (TID == null)
            {
                send.Add(new JsonStringValue("filename", "TID가 없음"));
                Response.Write(send.ToString());
                return;
            }


            String UID = coll.Get("UID");

            if (UID == null)
            {
                send.Add(new JsonStringValue("filename", "UID가 없음"));
                Response.Write(send.ToString());
                return;
            }

            //String[] arr1 = coll.AllKeys;

            /*
            int loop1, loop2;
            for (loop1 = 0; loop1 < arr1.Length; loop1++)
            {
                String Key = arr1[loop1];
                String Value;
                String[] arr2 = coll.GetValues( arr1[loop1] ) ;
                for (loop2 = 0; loop2 < arr2.Length; loop2++)
                {
                    Value = arr2[loop2];
                }
            }
            */


            int FileSize = file.ContentLength;
            if (file == null)
            {
                
                send.Add(new JsonStringValue("filename","파일이 없음" ));
                Response.Write(send.ToString());
                return;
            }

            String RecvName = file.FileName;

            String dir = "D://RtReplay//";
            String ext = ".replay";
            String filename = RecvName;

            String NewName = filename;
            String FullName = dir + NewName + ext;

            if (File.Exists(FullName))
            {

                NewName = DuplicateFile(dir, filename + ext);
                FullName = dir + NewName + ext;
            }



            try
            {
                file.SaveAs(FullName);
            }
            catch (Exception ex)
            {
                Response.Write(ex.Message);
                return;
            }


            String query = "CALL spAddRePlay (" + "'" + NewName + "'" + "," + "'" + FullName + "',"+ TID +","+ UID +")";



            DB db = new DB();
            OdbcDataReader reader = db.Exec(query);

           // JsonObjectCollection send = new JsonObjectCollection();
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

            send.Add(new JsonStringValue("filename", FullName));
            send.Add(new JsonStringValue("FileSize", FileSize.ToString()));

            reader.Close();
            db.Close();

            Response.Write(send.ToString());

        }
        public string DuplicateFile(string dir, string filename)
        {
            string file = Path.GetFileNameWithoutExtension(filename);
            string ext = Path.GetExtension(filename);

            string new_filename = dir + filename;
            string temp_filename = String.Empty;

            int cnt = 1;
            while (File.Exists(new_filename))
            {
                temp_filename = String.Format("{0}({1})", file, cnt++);
                new_filename = Path.Combine(dir + temp_filename + ext);
            }
            return temp_filename + ext;     // 파일명만 반환
        }
    }
}