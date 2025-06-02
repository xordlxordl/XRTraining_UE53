
using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Net.Json;
using System.Net.Sockets;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ARWeb
{
    public partial class download : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            String param = Request.Form[0];
            //String param = Request["nid"];
            JsonParser json = new JsonParser();



            String filename = json.Parse(param, "filename");
            String filetype = json.Parse(param, "filetype");

            String query = "CALL spGetFullPath('" + filename + "');";

            DB db = new DB();
            OdbcDataReader reader = db.Exec(query);

            String strFilePullPath = String.Empty;

            int nResult = 0;
            while (reader.Read())
            {
                String resultkey = "RESULT";
                String resultvalue = reader[resultkey].ToString();
                nResult = Convert.ToInt32(resultvalue);
                if( nResult == 1)
                {
                    String col = "fullpath";
                    String data = reader[col].ToString();
                    strFilePullPath = data;

                }
            }

            reader.Close();
            db.Close();

            byte[] bytes = Encoding.UTF8.GetBytes(filename);
            
            String endcodename = Convert.ToBase64String( bytes );




            if (nResult == 1)
            {
                Response.Headers.Add("filename", endcodename);
                Response.Headers.Add("filetype", filetype);

                Response.TransmitFile(strFilePullPath);
                Response.Flush();
                Response.End();
                
            }
        }
    }
}
