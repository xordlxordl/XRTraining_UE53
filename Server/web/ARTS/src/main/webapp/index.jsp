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
	
	<title>자율생성형 XR 훈련 시스템</title>


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
		<a class="navbar-brand" href="#"> <p class="fs-1">자율생성형 XR 훈련 시스템</p> </a>
		
			<!-- 화면 사이즈 줄어들면 드랍 메뉴가 버튼 안으로 들어가게 처리 하는 부분 -->
		<button class="navbar-toggler" type="button" data-bs-toggle="offcanvas" data-bs-target="#offcanvasNavbar2" aria-controls="offcanvasNavbar2">
			<span class="navbar-toggler-icon"></span>
		</button>
		<div class="offcanvas offcanvas-end text-bg-dark" tabindex="-1" id="offcanvasNavbar2" aria-labelledby="offcanvasNavbar2Label">
			
		<!-- 화면 사이즈가 줄어서 드랍메뉴가 버튼에 숨었을때 버튼을 누르면 오른쪽 사이드 바에 드랍메뉴가  뜨게 처리 하는 부분 -->
			
		</div>
	</div>
</header>
	

  

<body class="h-75 text-center text-bg-dark">


  <div class="cover-container d-flex w-100 p-3 mx-auto flex-column">
		<!-- 화면 사이즈 줄어들면 드랍 메뉴가 버튼 안으로 들어가게 처리 하는 부분 -->
  	<main class="m-auto">
    	<h1 class="display-2">자율생성형 XR 훈련</h1>
    	<section>
		<div class="container mt-5">
            <div class="row d-flex justify-content-center">
                <div class="col-md-8 col-xl-6">
                    <div class="card my-5 text-bg-light">
                    	<div class="card-header"> <h5>로그인 페이지 이동</h5></div>
                        <div class="card-body d-flex flex-column align-items-center">
                            
           					<a href="ar_login.jsp" type ="submit" class="btn btn-secondary border-white text-white bg-black btn-outline-light"
           					data-bs-toggle="tooltip"
           					data-bs-placement="bottom"
           					data-bs-custom-class="custom-tooltip"
        					data-bs-title="로그인 페이지로 이동합니다">로그인</a>
                            
                        </div>
                    </div>
                </div>
            </div>
        </div>
    	</section>
      		
  	</main>
</div>
		
<footer class="text-white-50 fixed-bottom">
	<%@ include file="foot.jspf" %>
</footer>


	
</body>
</html>
