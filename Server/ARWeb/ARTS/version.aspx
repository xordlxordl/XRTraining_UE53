<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="version.aspx.cs" Inherits="ARWeb.ARTS.version" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        <div>
             <asp:Label ID="strNetVersion" runat="server"></asp:Label>
            <br />
            <asp:TextBox ID="textboxNetVersion" runat="server"></asp:TextBox>
            <br />
            <input type="text" id="inputNetVersion" runat="server" size="100"/>
        </div>
    </form>
</body>
</html>
