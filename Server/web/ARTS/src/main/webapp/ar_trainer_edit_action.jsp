
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="aradmin" class="data.arAdmin" scope="page"/>
<jsp:setProperty name="aradmin" property="uid"/>
<jsp:setProperty name="aradmin" property="name"/>
<jsp:setProperty name="aradmin" property="pass"/>
<jsp:setProperty name="aradmin" property="group"/>



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
	Integer result = db.spUpdateTrainer( aradmin );
	
	if( result == 1 )
	{
		%>
		<script>AddTrainerOK( '<%=aradmin.getName()%> 수정 성공');</script>
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
	else if( result == -6 )
	{
		%>
		<script>MyError2Line('이미 있는 훈련자입니다','입력한 데이터를 확인 후 다시 진행 해주세요');</script>
		<%
	}
	else if( result == -100 )
	{
		%>
		<script>MyError('DB 오류');</script>
		<%
	}
%>



	

</body>


</html>
