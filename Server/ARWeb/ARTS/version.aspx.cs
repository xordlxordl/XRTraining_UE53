using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ARWeb.ARTS
{
    public partial class version : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            strNetVersion.Text = "닷넷 버전 :[" + System.Environment.Version.ToString() + "]";
            textboxNetVersion.Text = "닷넷 버전 :[" + System.Environment.Version.ToString() + "]";
            inputNetVersion.Value = "ASP.NET version " + System.Environment.Version.ToString();
        }
    }
}