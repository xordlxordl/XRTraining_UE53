<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>

<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.util.*"%>

<!DOCTYPE html>
<html class="h-100">

<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	
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

	
	<script src="js/my.js"></script>
	
	
	
	
	
	<title>DR TRANING SYTEM</title>
</head>


	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> 윤찬영</p> </a>
		
			<!-- 화면 사이즈 줄어들면 드랍 메뉴가 버튼 안으로 들어가게 처리 하는 부분 -->
		<button class="navbar-toggler" type="button" data-bs-toggle="offcanvas" data-bs-target="#offcanvasNavbar2" aria-controls="offcanvasNavbar2">
			<span class="navbar-toggler-icon"></span>
		</button>
		
	</div>
</header>
	

  

<body class="h-75 text-center text-bg-light">

<div id="page-wrapper">
  <!-- 사이드바 -->
  <!-- /사이드바 -->
  
  
	<section>
		<div class="container">
			<div class="row mb-3">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>윤찬영 정보</h1>
                </div>
            </div>
            	
                <br>
                <div class="form-row text-bg-light mx-auto">
					
					<form class="text-center" method="post" action="">
						<div>
						<img src="img/yoon.jpg" class="img-fluid img-thumbnail" alt="...">
						</div>
						 <div class="row input-group-text">
							<div class="form-group col-md-3">
				                <input type="text" class="form-control mx-3" value="윤찬영" >
			              	</div>
			              	
			              	
			              	<div class="form-group col-md-3 readonly">
			              	<div class="input-group-sm mx-2">
			              	<label class="mx-3">성별:</label>

								<div class="form-check form-check-inline required ">
									<input class="form-check-input" type="radio" name="nsex" id="inlineRadioMan" value="1" onclick="return false" readonly checked >
									<label class="form-check-label" for="inlineRadioMan">남</label>
								</div>
								<div class="form-check form-check-inline">
									<input class="form-check-input" type="radio" name="nsex" id="inlineRadioWoman" value="2" onclick="return false" readonly>
									<label class="form-check-label" for="inlineRadioWoman">여</label>
								</div>

							</div>
							</div>
							<div class="form-group col-md-3">
								<div class="input-group date datepicker" data-provide="datepicker">
						    		<input type="text" class="form-control datepicker" name = "birthdate" placeholder="생년월일" id="birthdatepicker" value ="1998 03 06" readonly>
						    		<div class="input-group-addon">
										<span class="glyphicon glyphicon-ko"></span>
						    		</div>
								</div>
			             	
			             	</div>
			             	<div class="form-group col-md-3">
			             	<div class="input-group">
			              		<label class="mx-3 mt-2">만</label>
			                	<input type="number" class="form-control mx-0" id="age" value="26" readonly" disabled>
			                	<label class="mx-1 mt-2 me-3">세</label>
			                </div>
			              	</div>
			              	   
						</div>
						
						<br>

						<div class="row input-group-text">

							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">소속학과:</label>
							</div>
						
							<div class="form-group col-md-2">
							  	<input type="text" class="form-control" value="컴퓨터 공학과" readonly>
							</div>
						</div>
						<br>

						<div class="row input-group-text">

							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">학년:</label>
							</div>
						
							<div class="form-group col-md-2">
							  	<input type="text" class="form-control" value="4학년" readonly>
							</div>
						</div>
						<br>
						

						<div class="row input-group-text">

							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">집위치:</label>
							</div>
						
							<div class="form-group col-md-8">
							
							  	<input type="text" class="form-control" value="대한민국 서울특별시 중랑구 동일로" readonly>

							</div>
						
			            </div>
			            <br>
			            
<!---------------------------------------------------------------------------------------------------------------------- -->
			            <div class="row input-group-text">

							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">회사위치:</label>
							</div>
						
							<div class="form-group col-md-8">
							
							  	<input type="text" class="form-control" value="인천광역시 연수구 갯벌로" readonly>

							</div>
						
			            </div>
						
						<br>
<!---------------------------------------------------------------------------------------------------------------------- -->
			            <div class="row input-group-text">

							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">담당업무:</label>
							</div>
						
							<div class="form-group col-md-8">
							
							  	<input type="text" class="form-control" value="서버 프로그래머" readonly>

							</div>
						
			            </div>
						
						<br>
<!---------------------------------------------------------------------------------------------------------------------- -->
			            <div class="row input-group-text">

							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">수강동기:</label>
							</div>
						
							<div class="form-group col-md-8">
							
							  	<input type="text" class="form-control" value="JSP 프로그래밍 기술을 습득하여 웹 개발 역량을 강화하고, 현업에서 활용할 수 있는 실무 능력을 키우고자 합니다." readonly>

							</div>
						
			            </div>
						
						<br>
<!---------------------------------------------------------------------------------------------------------------------- -->
			            <div class="row input-group-text">

							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">바라는 점:</label>
							</div>
						
							<div class="form-group col-md-8">
							
							  	<input type="text" class="form-control" value="실제 프로젝트에 적용할 수 있는 실용적인 지식과 최신 트렌드를 배우고, 동료 수강생들과의 네트워킹을 통해 다양한 경험을 공유하길 희망합니다." readonly>

							</div>
						
			            </div>
						
						<br>						
                        
					</form>
					
					
                
                </div>
            
        </div>
    </section>
  
  
</div>		



<br>		
<br>
<footer class="mt-auto text-black-50">
	<p>Copyright ©2024 by Yoon Chan Co.</a>, Ltd. All rights reserved.</p>
</footer>

<script src="js/dr_join.js"></script>

</body>


</html>
