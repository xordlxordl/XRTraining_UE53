<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="aradmin" class="data.arAdmin" scope="page"/>
<jsp:setProperty name="aradmin" property="account"/>
<jsp:setProperty name="aradmin" property="pass"/>

<!DOCTYPE html>
<html>
<head>

	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	
	<link rel="stylesheet" href="css/dark.css">
	<link rel="stylesheet" href="css/style.css">
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	
	<script src="js/sweetalert2.js"></script>
	
	<script src="js/my.js"></script>

	<meta charset="utf-8">
	
	<title>자율생성형 XR 훈련 시스템</title>
	
</head>

<body class="h-75 text-center text-bg-dark">
	<%
		int result = 0;
		
		arDBWork db = new arDBWork();
		arAdmin adminResult = db.spLogin( aradmin.getAccount(), aradmin.getPass());
		
		if( adminResult == null)
		{
			%>
			<script>LoginError('DB 프로시저 오류 NULL');</script>
			<%			
		}
		else
		{
			
			result = adminResult.getResult();
			if( result == 1 )
			{
		
				session.setAttribute("adminid", adminResult.getAccount() );
				session.setAttribute("admingrade", adminResult.getnGrade());
				%>
				<script>LoginOK( '<%=adminResult.getAccount()%> 로그인 성공');</script>
				<%
				
			}
			else if( result == 0 )
			{
				%>
				<script>LoginError('실패');</script>
				<%
			}
			
			else if( result == -2 )
			{
				%>
				<script>LoginError('계정이 없습니다.');</script>
				<%

			}
			else if( result == -4 )
			{
				%>
				<script>LoginError('비밀번호 오류');</script>
				<%
			}
		}
	%>
	
	
</body>
</html>
