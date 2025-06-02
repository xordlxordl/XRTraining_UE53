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
	
	%>
	<nav class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
		<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> XR Traninig System Main Page</p> </a>
		
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
						<li class="nav-item">
							<a class="nav-link" href="bbs.jsp">게시판</a>
						</li>
						<%
						if( strAdminID == null )
						{
								
						%>
						<script type="text/javascript">
							location.href = 'index.jsp';
						</script>
	
						<%
						}
						else
						{
						%>
							<li class="nav-item">
							<a class="nav-link" href="logoutAction.jsp">관리자 로그아웃</a>
							</li>

						<%
						}
						%>
						<form class="d-flex mt-3" role="search">
            				<input class="form-control me-2" type="search" placeholder="Search" aria-label="Search">
            				<button class="btn btn-secondary border-white text-white bg-black btn-outline-light w-50" type="submit">Search</button>
          				</form>
					</ul>
					
					
				</div>
			</div>
		</div>
	</nav>
	

  

	<div class="flex-column flex-shrink-0 p-3 text-bg-dark" style="width: 280px;">
		<a class="align-items-center mb-3 mb-md-0 me-md-auto text-white text-decoration-none">
		<span class="fs-4">관리메뉴</span></a>

	    <hr>
		    <ul class="nav nav-masthead flex-column mb-auto">
		      <li class="nav-item">
		        <a href="admin_main.jsp" class="nav-link text-white">
		          어드민
		        </a>
		      </li>
		      <li>
		        <a href="trainer_main.jsp" class="nav-link text-white">
		          관제자
		        </a>
		      </li>
		      <li>
		        <a href="user_main.jsp" class="nav-link text-white">
		          훈련자
		        </a>
		      </li>
		    </ul>
	    <hr>
    
	</div>
  
	
	<div class="cover-container d-flex w-100 p-3 mx-auto flex-column">
		<!-- 화면 사이즈 줄어들면 드랍 메뉴가 버튼 안으로 들어가게 처리 하는 부분 -->
		
  		<main class="px-3 m-auto">
    		<h1>XR Traninig System Main Page</h1>
    		<br>
    		<p class="lead">XR 훈련프로그램 관리 페이지 입니다.</p>
  		</main>

  		<footer class="mt-auto text-white-50 fixed-bottom">
    	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white">INTERACT Co.</a>, Ltd. All rights reserved.</p>
  		</footer>
	</div>
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
</body>
</html>
