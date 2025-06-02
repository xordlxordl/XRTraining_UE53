
using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ARWeb.ARTS
{
    public partial class download_voice : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            String param = Request.Form[0];
            //String param = Request["nid"];
            JsonParser json = new JsonParser();



            String i = json.Parse(param, "i");


            String query = "CALL spGetVoiceFile (" + i + ")";

            DB db = new DB();
            OdbcDataReader reader = db.Exec(query);

            String strFilePullPath = String.Empty;
            String filename = String.Empty;
            //int count = reader.FieldCount;
            while (reader.Read())
            {

                String col = "fullpath";
                String data = reader[col].ToString();
                strFilePullPath = data;
                filename = reader["filename"].ToString();

            }

            reader.Close();
            db.Close();

            byte[] bytes = Encoding.UTF8.GetBytes(filename);

            String endcodename = Convert.ToBase64String(bytes);

            Response.Headers.Add("filename", endcodename);
            Response.TransmitFile(strFilePullPath);
            Response.Flush();
            Response.End();
        }
    }
}