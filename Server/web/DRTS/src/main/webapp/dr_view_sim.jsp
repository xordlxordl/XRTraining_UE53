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
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/style.css">
	<link rel="stylesheet" href="css/sweetalert2.min.css">
	<link rel="stylesheet" href="css/form-validation.css">
	<link rel="stylesheet" href="css/bootstrap-datepicker3.css">
	<link rel="stylesheet" href="css/CircleSlide.css">

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


	<%
		Integer SelectCityIndex = 0;
		String strAdminID = null;
		Integer iAdminGrade = 100;
		
		if( session.getAttribute("adminid") != null )
		{
			strAdminID = (String)session.getAttribute("adminid");
			
		}
		String uid = null;
		if( request.getParameter("uid") != null )
			uid = ((String)request.getParameter("uid"));
		
		if( uid == null)
		{
			%>
			<script> UserMessageError("유저 정보를 불러오지 못했습니다."); </script>
			
			<%
			return;
		}
		
		DRUser user = new DRUser();
		DRDBWork db = new DRDBWork();
		user = db.GetTraineeData( uid );
		
	%>
	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="#"> <p class="fs-1"> 조건부 운전면허 시뮬레이터</p> </a>
		
		
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
						<script>
							location.href = 'index.jsp';
						</script>
		
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
	

  

<body class="h-75 text-center text-bg-light">

<div id="page-wrapper">
  <!-- 사이드바 -->
  <div id="sidebar-wrapper" class="flex-column flex-shrink-0 p-3 text-bg-dark">
  
    <ul class="sidebar-nav nav nav-masthead flex-column mb-auto">
      <li class="sidebar-brand">
        <a href="#" class="align-items-center mb-3 mb-md-0 me-md-auto text-white text-decoration-none">
        	<span class="fs-3">관리메뉴</span>
        </a>
      </li>
      <hr>
      
		<li class="nav-item"><a href="dr_user_main.jsp" class="nav-link text-white">수검자</a></li>
			<li class="nav-item">
				<a class="nav-link text-white fs-4 active" href="#">시뮬레이션</a>
			</li>
      <hr>
    </ul>
  </div>
  <!-- /사이드바 -->
  
  
	<section>
		<div class="container">
			<div class="row mb-3">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>시뮬레이션 환경설정</h1>
                </div>
            </div>
            	
                <br>
                
                <div class="form-row text-bg-light mx-auto bd-example-snippet bd-code-snippet">
					
					<form class="text-center" method="post" action="dr_UpdateSimAction.jsp">
						 <div class="row input-group-text">
						 	<div class="form-group col-md-1">
							<input type="text" class="form-control mx-3" name ="uid" value="<%=user.getUid()%>" readonly >
			              	</div>
							<div class="form-group col-md-2">
							
				            <input type="text" class="form-control mx-3 text-center"  placeholder="성명" readonly
				                value="<%=user.getSzname()%>"
				                >
			              	</div>
			              	
			              	
			              	<div class="form-group col-md-3">
			              	<div class="input-group-sm mx-2 input-group-text">
			              	<label>성별:</label>
									
								<div class="form-check form-check-inline mx-3">
									<input class="form-check-input" type="radio" id="inlineRadioMan" 
										<% if( user.getNsex() == 1 )
										{
										%>
										checked
										<%
										}
										%> 
									onclick="return false;" readonly>
									<label class="form-check-label" for="inlineRadioMan">남</label>
								</div>
								<div class="form-check form-check-inline">
									<input class="form-check-input" type="radio" id="inlineRadioWoman" 
									<% if( user.getNsex() == 2 )
										{
										%>
										checked
										<%
										}
										%>
									onclick="return false;" readonly>
									<label class="form-check-label" for="inlineRadioWoman">여</label>
								</div>

							</div>
							</div>
							<div class="form-group col-md-3">
								
						    		<input type="text" class="form-control text-center" readonly
						    		value="<%=user.getBirthdate() %>"
						    		>
						    		<div class="input-group-addon">
										<span class="glyphicon glyphicon-ko"></span>
						    		</div>
								
			             	
			             	</div>
			             	<div class="form-group col-md-3">
			             	<div class="input-group">
			              		<label class="mx-3 mt-2">만</label>
			                	<input type="number" class="form-control mx-0 text-center" readonly
			                	value="<%=user.GetAge()%>"
			                	>
			                	<label class="mx-1 mt-2 me-3">세</label>
			                </div>
			              	</div>
			              	   
						</div>
						
						<div class="row input-group-text mt-1">
							<div class="form-group col-md-1 mx-0">
								<label class="mt-1 float-start mx-3">거주지역:</label>
							</div>
							<div class="form-group col-md-6 float-start mx-3">
								<input type="text" class="form-control" readonly
						    		   value="<%=user.getSzaddr()%>" >

							</div>
			            </div>
			            <hr class="border border-primary border-2 opacity-75">
<!-- Calibration -->

		            	<div class="row input-group-text mt-1 dr_simulation">
							<div class="form-group col-md-2 mx-0">	
								<input type="button" id="id_CalibrationButton"
									class="btn btn-primary bg-gradient bg-opacity-75 btn-rounded" 
									onclick="StartCalibration()"
									value="Calibration">
								</input>
							</div>
							<div class="form-group col-md-8 mx-0">
								<div class="progress bg-white">
	  								<div class="progress-bar progress-bar-striped progress-bar-animated" 
		  								role="progressbar" aria-label="Animated striped example" 
		  								aria-valuenow="1" 
		  								aria-valuemin="0" aria-valuemax="100">
	  								</div>
								</div>
							</div>
							<div class="form-group col-md-2 mx-0">	
								<input type="button" id="id_NextButton"
									class="btn btn-danger" 
									onclick="NextCalibration()"
									value="Next" style="display:none">
								</input>
							</div>
						</div>
<!--  -->						
			            <hr class="border border-primary border-2 opacity-75">
			            
			              <div class="row input-group-text dr_simulation">
								<div class="form-group col-auto">
									<label class="float-start">모드 종류:</label>
								</div>
								
								<div class="form-group col-md-4 text-start float-start ">
				              		<div class="input-group-sm input-group-text">
				              	
										<div class="form-check form-check-inline">
											<input class="form-check-input dr_nocali" type="radio" name="mode" id="id_practice_mode" value="1" required>
											<label class="form-check-label" for="id_practice_mode">연습 모드</label>
										</div>
										<div class="form-check form-check-inline">
											<input class="form-check-input dr_nocali" type="radio" name="mode" id="id_driving_mode" value="2" required>
											<label class="form-check-label" for="id_driving_mode">주행 모드</label>
										</div>
	
									</div>
								</div>
								
								<div class="form-group col-md-2">
					              	<div class="input-group-sm  input-group-text">
		
										<div class="form-check form-check-inline">
											<input class="form-check-input dr_checkbox dr_class_driving_mode" type="checkbox" name="bAEBS" id="idaebs" value="1" disabled>
											<label class="form-check-label" for="idaebs">AEBS</label>
										</div>
									</div>
								</div>
							
						</div>
<!--1 row ----------------------------------------------------------------------------------------------------------------------------- -->
			         	
					
<!--2 row------------------------------------------------------------------------------------------------------------------------------->
						<div class="row dr_simulation">
							<div class="form-group col-md-8">
								<div class="row input-group-text mt-1 me-0">
									<div class="form-group col-md-1">
									<label class="float-start">시&nbsp;&nbsp;간&nbsp;&nbsp;대:</label>
									</div>
									
										<div class="form-group col-md-7 mx-5">
						              		<div class="input-group-sm input-group-text">

												<div class="form-check form-check-inline">
													<input class="form-check-input dr_class_driving_mode" type="radio" name="timezone" id="idtimeam" value="1" disabled required>
													<label class="form-check-label" for="idtimeam">오전</label>
												</div>
												<div class="form-check form-check-inline">
													<input class="form-check-input dr_class_driving_mode" type="radio" name="timezone" id="idtimepm" value="2" disabled>
													<label class="form-check-label" for="idtimepm">오후</label>
												</div>
				
												<div class="form-check form-check-inline">
													<input class="form-check-input dr_class_driving_mode" type="radio" name="timezone" id="idtimeevening" value="3" disabled>
													<label class="form-check-label" for="idtimeevening">저녁</label>
												</div>
												
												<div class="form-check form-check-inline">
													<input class="form-check-input dr_class_driving_mode" type="radio" name="timezone" id="idtimenight" value="4" disabled>
													<label class="form-check-label" for="idtimenight">심야</label>
												</div>
			
											</div>
										</div>
								</div>
								
								 <div class="row input-group-text mt-1 me-0">
									<div class="form-group col-md-1">
									<label class="float-start">기&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;상:</label>
									</div>
									
									<div class="form-group col-7 mx-5 text-start ">
						              	<div class="input-group-sm input-group-text">
						              	
			
											<div class="form-check form-check-inline">
												<input class="form-check-input dr_class_driving_mode" type="radio" name="weather" id="idweathersun" value="1" disabled required>
												<label class="form-check-label" for="idweathersun">맑음</label>
											</div>
											<div class="form-check form-check-inline">
												<input class="form-check-input dr_class_driving_mode" type="radio" name="weather" id="idweatherrain" value="2" disabled>
												<label class="form-check-label" for="idweatherrain">강우</label>
											</div>
			
											<div class="form-check form-check-inline">
												<input class="form-check-input dr_class_driving_mode" type="radio" name="weather" id="idweathersnow" value="3" disabled>
												<label class="form-check-label" for="idweathersnow">강설</label>
											</div>
											
											<div class="form-check form-check-inline">
												<input class="form-check-input dr_class_driving_mode" type="radio" name="weather" id="idweatherfog" value="4" disabled>
												<label class="form-check-label" for="idweatherfog">안개</label>
											</div>
			
										</div>
									</div>
							
								</div>
								
								<div class="row mt-1 input-group-text me-0">
							
								<div class="col-md-2 ">
									<label class="float-start align-middle">기상 세부:</label >
								</div>
								
<!-- card start ------------------------------------------------------------------------------------------------------------------------------------------>
								<div class="col-md-9 card mx-2">
									<div class="row mt-1 mx-0">
										<div class="col-md-2">
										<label for="IDRangeAmount" class="form-label mt-2">Amount</label>
										</div>
										<div class="col-md-7">
										<input type="range" min="0" max="100" class="form-range mt-2 dr_amount" id="IDRangeAmount" oninput="IDEditAmouont.value = IDRangeAmount.value" value="0" disabled>
										</div>
										<div class="col-md-3">
										<input type="number" min="0" max="100" class="form-control form-control-sm text-center mt-1 dr_amout" id="IDEditAmouont" name="amount" placeholder="0~100" oninput="IDRangeAmount.value = IDEditAmouont.value" disabled>
										</div>
										
										<div class="col-md-2">
										<label for="IDRangeWind" class="form-label mt-2">풍속(m/s)</label>
										</div>
										<div class="col-md-7">
										<input type="range" min="0" max="45" class="form-range mt-2 dr_class_driving_mode" id="IDRangeWind" oninput="IDEditWind.value = IDRangeWind.value" value="0" disabled>
										</div>
										<div class="col-md-3">
										<input type="number" 
											min="0" 
											max="45" 
											class="form-control form-control-sm text-center mt-1 dr_class_driving_mode" 
											id="IDEditWind" 
											name="windspeed" 
											placeholder="0~45" 
											oninput="IDRangeWind.value = IDEditWind.value" 
											disable>
										</div>
									</div>
								</div>
<!-- card end-------------------------------------------------------------------------------------------------------------------------------------- -->

							</div>
								
						</div>
							
							
							<div class="col-md-4 mt-1">
								<div class="row input-group-text">
									<div class="col-8">
		                                   	<div id="circle">
			                                   	
												<div class="fs-3" id="arrow">&#9660;</div>
												<div id="circle1"></div>
												<div id="circle2"></div>
												
											</div>
									</div>
									<div class="col-4">
										<label>풍향</label>
										<input type="text" class="form-control form-control-sm text-center" id="IDEditWindPos" name="winddir" value="0" placeholder="" disabled readonly>
									</div>			
											
								</div>
							</div>
							
						</div><!-- <div class="row dr_simulation"> -->


						<br>
						<div class="row mt-1">
							<div class="col-md-3"></div>
							<div class="col-md-3">
								<!-- DRServer.js -->
								<button class="btn btn-primary" id="id_simsetbutton_ok" type="button" onclick="SendSetOption()">세팅</button>
							</div>
							<div class="col-md-3">
	                        	<button class="btn btn btn-outline-danger" onclick ="CancleSimSet()" type="button">취소</button>
							</div>
							
							<div class="col-md-3"></div>
							
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
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-black navbar-brand">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>


<script src="js/dr_setsim.js"></script>
<script src="js/CircleSlide.js"></script>
<script src="js/Calibration.js"></script>
<script src="js/DRServer.js"></script>


</body>


</html>
