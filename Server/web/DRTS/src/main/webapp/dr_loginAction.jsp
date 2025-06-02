<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.io.PrintWriter" %>
<%
request.setCharacterEncoding("UTF-8");
%>

<jsp:useBean id="dradmin" class="data.DRAdmin" scope="page"/>
<jsp:setProperty name="dradmin" property="drid"/>
<jsp:setProperty name="dradmin" property="drpass"/>

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
<title>DR TRANING SYTEM</title>
</head>
<body class="h-75 text-center text-bg-dark">

	<%
	int result = 0;
		String strAdminID = null;
		Integer iAdminGrade = 100;
		String strIP = request.getRemoteAddr();
		if( session.getAttribute("adminid") != null )
		{
			strAdminID = (String)session.getAttribute("adminid");
		}
		/*
		if( session.getAttribute("admingrade") != null )
		{
			iAdminGrade = ((Integer)session.getAttribute("admingrade")).intValue();
		}
		*/
		if( strAdminID != null )
		{
			PrintWriter script = response.getWriter();
			script.println("<script>");
			
			
			script.println("location.href = 'dr_user_main.jsp'");
			
			
			script.println("</script>");
		}
		
		DRDBWork db = new DRDBWork();
		DRAdmin adminResult = db.login( dradmin.getDrid(), dradmin.getDrpass(), strIP);
		
		if( adminResult == null)
		{
			PrintWriter script = response.getWriter();
			script.println("<script>");
			script.println("alert('데이타 베이스 에러')");
			script.println("history.back()");
			script.println("</script>");
			
		}
		else
		{
			//결과에는 아이디가 없으므로 입력받은 아이디를 대입
			adminResult.setDrid( dradmin.getDrid() );
			//;
			
			result = adminResult.getResult();
			if( result == 1 )
			{
		
				session.setAttribute("adminid", adminResult.getDrid());
				
				PrintWriter script = response.getWriter();
				script.println("<script>");
				String x;
				
				
				x = "location.href = 'dr_user_main.jsp?myresult=1&name=" + adminResult.getDrid() + "'";
				
				
				
				
				script.println(x);
				script.println("</script>");
			}
			else if( result == 0 )
			{
	%>
				<script>MyError('실패');</script>
				<%
			}
			
			else if( result == 2 )
			{
				%>
				<script>MyError('계정이 없습니다.');</script>
				<%

			}
			else if( result == 3 )
			{
				%>
				<script>MyError('비밀번호 오류');</script>
				<%
			}
			else if( result == 4 )
			{
				%>
				<script>MyError('삭제 대기 중인 계정');</script>
				<%
			}
			
		}
	%>
	
	
</body>
</html>
