<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>

<!DOCTYPE html>
<html class="h-100">

<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/style.css">
	<link rel="stylesheet" href="css/sweetalert2.min.css">
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/my.js"></script>
	
	<title>XR TRANING SYTEM</title>
</head>	

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
	%>
	
	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> XR Traninig System</p> </a>
		
			<!-- 화면 사이즈 줄어들면 드랍 메뉴가 버튼 안으로 들어가게 처리 하는 부분 -->
		<button class="navbar-toggler" type="button" data-bs-toggle="offcanvas" data-bs-target="#offcanvasNavbar2" aria-controls="offcanvasNavbar2">
			<span class="navbar-toggler-icon"></span>
		</button>
		<div class="offcanvas offcanvas-end text-bg-dark" tabindex="-1" id="offcanvasNavbar2" aria-labelledby="offcanvasNavbar2Label">
			
		<!-- 화면 사이즈가 줄어서 드랍메뉴가 버튼에 숨었을때 버튼을 누르면 오른쪽 사이드 바에 드랍메뉴가  뜨게 처리 하는 부분 -->
			<div class="offcanvas-header">
				<h5 class="offcanvas-title" id="offcanvasNavbar2Label">접속하기</h5>
				<button type="button" class="btn-close btn-close-white" data-bs-dismiss="offcanvas" aria-label="Close"></button>
			</div>
				
			<div class="offcanvas-body">
				<ul class="navbar-nav nav-masthead justify-content-end flex-grow-1">
					<li class="nav-item active">
						<a class = "nav-link active" aria-current="page" href="main.jsp">메인</a>
					</li>
				</ul>
			</div>
		</div>
	</div>
</header>
	

  

<body class="h-75 text-center text-bg-dark">





  <div class="cover-container d-flex w-100 p-3 mx-auto flex-column">
		<!-- 화면 사이즈 줄어들면 드랍 메뉴가 버튼 안으로 들어가게 처리 하는 부분 -->
  	<main class="px-3 m-auto">
    	<h1>XR Traninig System Admin Page</h1>
    		<br>
    		<p class="lead">XR 훈련프로그램 메인 페이지 입니다.</p>
    		<p class="lead">관리 페이지로 이동하려면 로그인을 하세요.</p>
    		<br>
      		<a href="login.jsp" type ="submit" class="btn btn-secondary border-white text-white bg-black btn-outline-light">로그인</a>
      		<a href="join_userself.jsp" class="btn btn-secondary border-white bg-white">훈련자 개인 등록</a>
  	</main>
</div>
	
<%
if( request.getParameter("myresult") != null )
{
	
	if( request.getParameter("myresult").equals("-1") )
	{
%>
	<script>MyAlert('알수 없는 오류');</script>
<%
	}
	else if( request.getParameter("myresult").equals("-2") )
	{
%>
	<script>MyAlert('계정이 없습니다');</script>
<%
	}
	else if( request.getParameter("myresult").equals("-3") )
	{
%>
	<script>MyAlert('권한이 없습니다');</script>
<%
	}
	else if( request.getParameter("myresult").equals("-4") )
	{
%>
	<script>MyAlert('비밀 번호 오류');</script>
<%
	}
	else if( request.getParameter("myresult").equals("-100") )
	{
%>
	<script>MyAlert('시스템 오류');</script>
<%
	}
	else if( request.getParameter("myresult").equals("logout") )
	{
%>
	<script>MyAlert('로그아웃');</script>
<%
	}
}
%>
			
<footer class="mt-auto text-white-50">
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>
	

	
</body>
</html>
