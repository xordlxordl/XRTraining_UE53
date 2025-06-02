
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="druser" class="data.DRUser" scope="page"/>
<jsp:setProperty name="druser" property="uid"/>
<jsp:setProperty name="druser" property="szname"/>
<jsp:setProperty name="druser" property="nsex"/>
<jsp:setProperty name="druser" property="birthdate"/>
<jsp:setProperty name="druser" property="nsi"/>
<jsp:setProperty name="druser" property="ngun"/>
<jsp:setProperty name="druser" property="ndong"/>
<jsp:setProperty name="druser" property="bsick"/>
<jsp:setProperty name="druser" property="szsick"/>
<jsp:setProperty name="druser" property="bdrug"/>
<jsp:setProperty name="druser" property="szdrug"/>
<jsp:setProperty name="druser" property="careeryear"/>
<jsp:setProperty name="druser" property="careermonth"/>
<jsp:setProperty name="druser" property="bdrive"/>
<jsp:setProperty name="druser" property="bcrash"/>
<jsp:setProperty name="druser" property="bcrashobject"/>
<jsp:setProperty name="druser" property="bcrashhuman"/>
<jsp:setProperty name="druser" property="bcrashonly"/>

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
	
	


	<script src="js/bootstrap-datepicker.min.js"></script>
	<script src="locales/bootstrap-datepicker.ko.min.js"></script>

	<script src="js/SelectSiDo.js"></script>
	<script src="js/SelectGunGu.js"></script>
	
	
	
	<script src="js/my.js"></script>
	
	<title>DR TRANING SYTEM</title>
</head>
<body class="h-75 text-center text-bg-dark">


<%
	DRDBWork db = new DRDBWork();
	Integer nResult = db.UpdateTrainee( druser );
	
	if( nResult == null )
	{
		%>
		<script>MyError('DB 오류 Data NULL');</script>
		<%
		return;
	}
	
	if( nResult == 1 )
	{
		%>
		<script>UpdateUserMessageOK( '<%=druser.getSzname()%> 수정 성공' );</script>
		<%
	}
	else if( nResult == 0 )
	{
		%>
		<script>MyError('실패');</script>
		<%
	}

%>

	

</body>


</html>
