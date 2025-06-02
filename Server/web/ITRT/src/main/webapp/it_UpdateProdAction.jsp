
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="itprod" class="data.ITProd" scope="page"/>
<jsp:setProperty name="itprod" property="pid"/>
<jsp:setProperty name="itprod" property="name"/>
<jsp:setProperty name="itprod" property="desc"/>


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
	Integer nResult = db.UpdateProd( itprod );
	
	if( nResult == 1 )
	{
		%>		
		<script>UpdateProdMessageOK('<%= itprod.getName() %> 수정 완료')</script>
		<%
	}
	else if( nResult == 0 )
	{
		%>
		<script>UpdateProdError('실패','<%= itprod.getPid() %>');</script>
		<%
	}
	else if( nResult == -1 )
	{
		%>
		<script>UpdateProdError('제품 이름 중복','<%= itprod.getPid() %>');</script>
		<%
	}
	
%>


	
	
	
	

</body>


</html>
