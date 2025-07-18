<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>

<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<% request.setCharacterEncoding("UTF-8"); %>

<jsp:useBean id="xr" class="data.Server" scope="page"/>
<jsp:setProperty name="xr" property="nid"/>
<jsp:setProperty name="xr" property="mapid"/>
<jsp:setProperty name="xr" property="name"/>
<jsp:setProperty name="xr" property="ip"/>
<jsp:setProperty name="xr" property="port"/>

<!DOCTYPE html>
<html>
<head>

	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/style.css">
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/my.js"></script>

<meta charset="utf-8">
<title>XR TRANING SYTEM</title>
</head>
<body class="h-75 text-center text-bg-dark">
	<%
	String strAdminID = null;
	Integer iAdminGrade = 100;
			
	if( session.getAttribute("adminid") != null )
	{
		strAdminID = (String)session.getAttribute("adminid");
		
		if( session.getAttribute("admingrade") != null )
		{
			iAdminGrade = ((Integer)session.getAttribute("admingrade")).intValue();
		}
	}
	else
	{
		PrintWriter script = response.getWriter();
		script.println("<script>");
		script.println("location.href = 'index.jsp'");
		script.println("</script>");
	}
			
			
		
			
	DBWork db = new DBWork();
	int result = db.CreateServer( xr.getNid(), xr.getMapid(), xr.getName(), xr.getIp(), xr.getPort() );
			
	if( result == 1 )
	{
		PrintWriter script = response.getWriter();
		script.println("<script>");
		String x;
		x = "location.href = 'add_server.jsp?myresult=1&name=" + xr.getName() + "'";
		script.println( x );
		script.println("</script>");
	}
	else if( result == 0 )
	{
		%>
			<script>MyError('실패');</script>
		<%
	}
	else if( result == -1 )
	{
		%>
			<script>MyError('알 수 없는 오류');</script>
		<%

	}
	else if( result == -2 )
	{
		%>
			<script>MyError('계정이 없습니다.');</script>
		<%

	}
	else if( result == -3 )
	{
		%>
			<script>MyError('권한이 없습니다.');</script>
		<%
	}
	else if( result == -4 )
	{
		%>
			<script>MyError('비밀번호 오류');</script>
		<%
	}
	else if( result == -5 )
	{
		%>
			<script>MyError('아이디 중복 오류');</script>
		<%
	}
	else if( result == -6 )
	{
		%>
			<script>MyError('XR번호 중복 오류');</script>
		<%
	}
	else if( result == -7 )
	{
		%>
			<script>MyError('XR클라이언트 기기 이름 중복 오류');</script>
		<%
	}
	else if( result == -8 )
	{
		%>
			<script>MyError('서버ID 중복 오류');</script>
		<%
	}
	else if( result == -9 )
	{
		%>
			<script>MyError('동일한 IP와 PORT를 가진 서버가 있습니다');</script>
		<%
	}
	else if( result == -99 )
	{
		%>
			<script>MyError('내용을 입력해 주세요');</script>
		<%

	}
	else if( result == -100 )
	{
		%>
			<script>MyError('시스템 오류');</script>
		<%

	}
	%>
</body>
</html>
