
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="ituser" class="data.ITUser" scope="page"/>
<jsp:setProperty name="ituser" property="uid"/>
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
	
	Integer nResult = 0;
	
	if( ituser.getPass() == null)
	{
		nResult = db.UpdateUser( ituser, 0 );	
	}
	else
	{
		nResult = db.UpdateUser( ituser, 1 );
	}
	
	
	
	if( nResult == 1 )
	{
		%>
		<script> UpdateUserMessageOK('<%= ituser.getLoginid() %>수정 완료');</script>
		<%
	}
	else if( nResult == 0 )
	{
		%>
		<script>UpdateUserError('실패','<%=ituser.getUid() %>');</script>
		<%
	}
	else if( nResult == -2 )
	{
		%>
		<script>UpdateUserError('로그인 계정 중복','<%=ituser.getUid() %>');</script>
		<%
	}
	else if( nResult == -3 )
	{
		%>
		<script>UpdateUserError('없는 조직','<%=ituser.getUid() %>');</script>
		<%
	}
%>


	
	
	
	

</body>


</html>
