
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="arCSV" class="data.arCSV" scope="page"/>
<jsp:setProperty name="arCSV" property="query"/>



<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/dark.css">
	<link rel="stylesheet" href="css/style.css">
	
	<script src="js/jquery-3.6.4.js"></script>
	<script src="js/jquery-ui.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.js"></script>
	
	<script src="js/my.js"></script>
	
	<title>자율생성형 XR 훈련 시스템</title>
</head>
<body class="h-75 text-center text-bg-dark">


<%
	arDBWork db = new arDBWork();
	arResult iResult = db.InsertUserQuery( arCSV.getQuery() );
	
	if( iResult.getnResult() > 0 )
	{
		%>
		<script>AddUserOK( '<%=iResult.getnResult()%>명 훈련자 생성 성공');</script>
		<%
	}
	else if( iResult.getnResult() == 0 )
	{
		%>
		<script>LoginError( "<%=iResult.getStrError()%>생성 실패");</script>
		<%
	}
	else if( iResult.getnResult() == -1 )
	{
		%>
		<script>LoginError( "<%=iResult.getStrError()%>");</script>
		<%
	}
%>



	

</body>


</html>
