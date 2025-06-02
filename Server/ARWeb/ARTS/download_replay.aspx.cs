using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ARWeb.ARTS
{
    public partial class download_replay : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            String param = Request.Form[0];
            //String param = Request["nid"];
            JsonParser json = new JsonParser();



            String nID = json.Parse(param, "nid");


            String query = "CALL spGetReplayFile (" + nID + ")";

            DB db = new DB();
            OdbcDataReader reader = db.Exec(query);

            String strFilePullPath = String.Empty;

            int count = reader.FieldCount;
            while (reader.Read())
            {

                String col = "FullPath";
                String data = reader[col].ToString();
                strFilePullPath = data;


            }

            reader.Close();
            db.Close();

            Response.TransmitFile( strFilePullPath );
            Response.Flush();
            Response.End();
        }
    }
}