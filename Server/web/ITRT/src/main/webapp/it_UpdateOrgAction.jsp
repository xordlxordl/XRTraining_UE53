
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="itorg" class="data.ITOrg" scope="page"/>
<jsp:setProperty name="itorg" property="oid"/>
<jsp:setProperty name="itorg" property="type"/>
<jsp:setProperty name="itorg" property="name"/>
<jsp:setProperty name="itorg" property="memo"/>


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
	Integer nResult = db.UpdateOrg( itorg );
	
	if( nResult == 1 )
	{
		%>		
		<script>UpdateOrgMessageOK('<%= itorg.getName() %> 수정 완료')</script>
		<%
	}
	else if( nResult == 0 )
	{
		%>
		<script>UpdateOrgError('실패','<%= itorg.getOid() %>');</script>
		<%
	}
	else if( nResult == -1 )
	{
		%>
		<script>UpdateOrgError('조직 이름 중복','<%= itorg.getOid() %>');</script>
		<%
	}
	else if( nResult == -3 )
	{
		%>
		<script>UpdateOrgError('없는 조직','<%= itorg.getOid() %>');</script>
		<%
	}
%>
	

</body>


</html>
