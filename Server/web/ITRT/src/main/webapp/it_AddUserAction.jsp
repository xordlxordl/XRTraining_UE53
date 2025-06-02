
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="ituser" class="data.ITUser" scope="page"/>
<jsp:setProperty name="ituser" property="oid"/>
<jsp:setProperty name="ituser" property="loginid"/>
<jsp:setProperty name="ituser" property="pass"/>


<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/style.css">
	<link rel="stylesheet" href="css/sweetalert2.min.css">
	<link rel="stylesheet" href="css/form-validation.css">
	<link rel="stylesheet" href="css/bootstrap-datepicker3.css">

	<script src="js/jquery-3.6.4.js"></script>
	<script src="js/jquery-ui.min.js"></script>
	
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/my.js"></script>
	
	<title>INTERACT SYTEM</title>
</head>
<body class="h-75 text-center text-bg-light">


<%
	ITDBWork db = new ITDBWork();
	Integer nResult = db.AddUser( ituser );
	
	if( nResult == 1 )
	{
		PrintWriter script = response.getWriter();
		script.println("<script>");
		
		String x;
		x = "location.href = 'it_add_user.jsp?myresult=1&name=" + ituser.getLoginid() + "'";
		script.println( x );
		
		script.println("</script>");
	}
	else if( nResult == 0 )
	{
		%>
		<script>AddUserError('실패');</script>
		<%
	}
	else if( nResult == -2 )
	{
		%>
		<script>AddUserError('로그인 계정 중복');</script>
		<%
	}
	else if( nResult == -3 )
	{
		%>
		<script>AddUserError('없는 조직');</script>
		<%
	}
%>
	

</body>


</html>
