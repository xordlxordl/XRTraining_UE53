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
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>	
	
	<script src="js/my.js"></script>
	<title>DR TRANING SYTEM</title>
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
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> 조건부 운전면허 시뮬레이터 환경설정 시스템</p> </a>
		
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
					
					
					<%
						if( strAdminID == null )
						{
							
					%>
					<li class="nav-item dropdown">
						<a class="nav-link dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown" aria-expanded="false">
						접속하기
						</a>
						<ul class="dropdown-menu dropdown-menu-dark gap-1 p-2 rounded-3 mx-0 border-0 shadow w-220px">
								<li><a class="dropdown-item d-flex align-items-center gap-2 py-2" href="login.jsp">관리자 로그인</a></li>
						</ul>
					</li>

					<%
					}
					else
					{
					%>
						<li class="nav-item">
							<a class="nav-link" href="dr_logoutAction.jsp">관리자 로그아웃</a>
						</li>

					<%
					}
					%>
				</ul>
			</div>
		</div>
	</div>
</header>
	

  

<body class="h-75 text-center text-bg-dark">

	<section>
		<div class="container">
			
            <div class="row d-flex justify-content-center">
                <div class="col-md-6 col-xl-4">
                    <div class="card my-5 text-bg-light">
                    	<div class="card-header"> <h3>관리자 로그인</h3></div>
                        <div class="card-body d-flex flex-column align-items-center">
                            <div class="my-3"></div>
                            
                            <form class="text-center" method="post" action="dr_loginAction.jsp">
                                <div class="mb-3"><input class="form-control" type="text" name="drid" placeholder="아이디" maxlength="15"></div>
                                <div class="mb-3"><input class="form-control" type="password" name="drpass" placeholder="비밀번호" maxlength="15"></div>
                                <div class="mb-3"><button class="btn btn-secondary border-white text-white bg-black btn-outline-light w-100" type="submit">로그인</button></div>
                            </form>
                            
                            <div id="liveAlertPlaceholder"></div>
                            <div class="my-2"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
		
			
<footer class="mt-auto text-white-50 fixed-bottom">
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white navbar-brand">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>
	

</body>
</html>
