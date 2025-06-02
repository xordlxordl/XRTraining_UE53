
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="ituserprod" class="data.ITUserProductData" scope="page"/>

<jsp:setProperty name="ituserprod" property="uid"/>
<jsp:setProperty name="ituserprod" property="pid"/>

<jsp:setProperty name="ituserprod" property="createat"/>
<jsp:setProperty name="ituserprod" property="expiredat"/>
<jsp:setProperty name="ituserprod" property="memo"/>

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
ITResult info = db.AddUserProd( ituserprod );

if( info == null )
{
	%>
	<script>AddUserProdError('실패');</script>
	<%
}
else
{
	Integer nResult = info.getResult(); 
	if( nResult == 1 )
	{
		%>
		<script>AddUserProdMessageOK2('<%= info.getLoginid()%> <%=info.getProdname()%> 제품 등록 완료', '<%= ituserprod.getUid() %>');</script>
		<%
	}
	else if( nResult == 0 )
	{
		%>
		<script>AddUserProdError('실패');</script>
		<%
	}
	else if( nResult == -2 )
	{
		%>
		<script>AddUserProdError('이미 등록된 제품');</script>
		<%
	}
}
	
%>


	
	
	
	

</body>


</html>
