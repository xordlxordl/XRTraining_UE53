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
	
	%>
	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> DR Traninig System</p> </a>
		
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
					<script type="text/javascript">
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
				<a class="nav-link text-white fs-4 active" href="join_user.jsp">등록</a>
			</li>
      <hr>
    </ul>
  </div>
  <!-- /사이드바 -->
  
  
	<section>
		<div class="container">
			<div class="row mb-3">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>신규 수검자 정보</h1>
                </div>
            </div>
            	
                <br>
                <div class="form-row text-bg-light mx-auto">
					
					<form class="text-center" method="post" action="dr_JoinUserAction.jsp">
						 <div class="row input-group-text">
							<div class="form-group col-md-3">
				                <input type="text" class="form-control mx-3" name = "szname" id="username" placeholder="성명" required>
			              	</div>
			              	
			              	
			              	<div class="form-group col-md-3">
			              	<div class="input-group-sm mx-2">
			              	<label class="mx-3">성별:</label>

								<div class="form-check form-check-inline required">
									<input class="form-check-input" type="radio" name="nsex" id="inlineRadioMan" value="1" required>
									<label class="form-check-label" for="inlineRadioMan">남</label>
								</div>
								<div class="form-check form-check-inline">
									<input class="form-check-input" type="radio" name="nsex" id="inlineRadioWoman" value="2">
									<label class="form-check-label" for="inlineRadioWoman">여</label>
								</div>

							</div>
							</div>
							<div class="form-group col-md-3">
								<div class="input-group date datepicker" data-provide="datepicker">
						    		<input type="text" class="form-control datepicker" name = "birthdate" placeholder="생년월일" id="birthdatepicker" required>
						    		<div class="input-group-addon">
										<span class="glyphicon glyphicon-ko"></span>
						    		</div>
								</div>
			             	
			             	</div>
			             	<div class="form-group col-md-3">
			             	<div class="input-group">
			              		<label class="mx-3 mt-2">만</label>
			                	<input type="number" class="form-control mx-0" id="age" placeholder="나이" disabled>
			                	<label class="mx-1 mt-2 me-3">세</label>
			                </div>
			              	</div>
			              	   
						</div>
						
						<br>
						
						<div class="row input-group-text">
							<div class="form-group col-md-2 mx-0">
							<label class="mt-1 float-start mx-3">거주지역:</label>
							</div>
							<div class="form-group col-md-3">
							
							<select id="city" class="form-select mx-3" name = "nsi" onchange="changeCityFn()" required>
							  	<option value="">시/도</option>
							  	<%
							  	DRDBWork db = new DRDBWork();
								ArrayList<DRJuso> list = db.getJuso1();
								int i;
								for( i = 0; i < list.size(); ++i )
								{
							  	%>
							  	<option value=<%= i+1 %> > <%= list.get(i).getName() %></option>
							  	<%
								}
							  	%>
								
							</select>
							</div>
						
							<div class="form-group col-md-3">
							<select id="gungu" class="form-select mx-3" name = "ngun" onchange="changeGunGuFn()" required>
							  	<option value="">시/군/구</option>
								<!-- 이부분은 SelectSiDo.js 에서 만들어줌 -->
							</select>
							</div>
							
							<div class="form-group col-md-3 mx-3">
							<select id="dong" class="form-select" name = "ndong" required>
							  	<option value="">읍/면/동</option>
							  	<!-- 이부분은 SelectGunGu.js 에서 만들어줌 -->
							</select>
							</div>

						
			            </div>
			            
			            <br>
			            
						<div class="row input-group-text">
							<div class="form-group col-md-4 text-start">
			              		<div class="input-group-sm ">
			              		<label class="mx-3">질&nbsp; &nbsp;환:</label>
									<div class="form-check form-check-inline">
										<input class="form-check-input" type="radio" name="bsick" id="inlineRadioNonSick" value="0" required>
										<label class="form-check-label" for="inlineRadioNonSick">없음</label>
									</div>
									<div class="form-check form-check-inline">
										<input class="form-check-input" type="radio" name="bsick" id="inlineRadioSick" value="1">
										<label class="form-check-label" for="inlineRadioSick">있음</label>
										
									</div>
								</div>
							</div>
							<div class="form-group col-md-6">
				                <input type="text" class="form-control text-center" name="szsick" id="sicktext" placeholder="2주내 진단 받은 질환" disabled required>
			              	</div>
							
						</div>
						    
						
						<div class="row mt-2 input-group-text">
							<div class="form-group col-md-4 text-start">
			              		<div class="input-group-sm ">
			              		<label class="mx-3">복용약:</label>
									<div class="form-check form-check-inline">
										<input class="form-check-input" type="radio" name="bdrug" id="inlineRadioNonDrug" value="0" required>
										<label class="form-check-label" for="inlineRadioNonDrug">없음</label>
									</div>
									<div class="form-check form-check-inline">
										<input class="form-check-input" type="radio" name="bdrug" id="inlineRadioDrug" value="1">
										<label class="form-check-label" for="inlineRadioDrug">있음</label>
										
									</div>
								</div>
							</div>
							<div class="form-group col-md-6">
				                <input type="text" class="form-control text-center me-5" name ="szdrug" id="drugtext" placeholder="" disabled required>
			              	</div>
						</div>
						
						<br>
						<div class="row input-group-text">
						<div class="form-group col-md-2 mx-0">
							<label class="float-start mx-3 mt-1">운전경력:</label>
						</div>
						
							<div class="form-group col-md-2 mx-0">
			             	<div class="input-group ">
			                	<input type="number" class="form-control" name="careeryear" id="career-year" placeholder="">
			                	<label class="mx-1 mt-2 me-3">년</label>
			                	
			                </div>
			              	</div>
			              	<div class="form-group col-md-2 mx-0">
			             	<div class="input-group">
			                	<input type="number" class="form-control" name ="careermonth" id="career-month" placeholder="">
			                	<label class="mx-1 mt-2 me-3 ">개월</label>
			                </div>
			              	</div>
			            	
						</div>
						
						<br>
						<div class="row input-group-text">
							<div class="form-group col-md-2">
							<label class="mx-3 float-start">3개월 이내:</label>
							</div>
							<div class="form-group col-md-4 mx-0 offset-md-2">
			              	<div class="input-group-sm input-group-text">
			              	<label class="mx-1">운전경험: &nbsp; &nbsp; </label>

								<div class="form-check form-check-inline">
									<input class="form-check-input" type="radio" name="bdrive" id="inlineRadioNonDrive" value="0" required>
									<label class="form-check-label" for="inlineRadioNonDrive">없음</label>
								</div>
								<div class="form-check form-check-inline">
									<input class="form-check-input" type="radio" name="bdrive" id="inlineRadioDrive" value="1">
									<label class="form-check-label" for="inlineRadioDrive">있음</label>
								</div>

							</div>
							</div>
						
							<div class="form-group col-md-4 mx-0">
			              	<div class="input-group-sm  input-group-text">
			              	<label class="mx-1">사고경험: &nbsp; &nbsp; </label>

								<div class="form-check form-check-inline">
									<input class="form-check-input" type="radio" name="bcrash" id="inlineRadioNonCrash" value="0" required>
									<label class="form-check-label" for="inlineRadioNonCrash">없음</label>
								</div>
								<div class="form-check form-check-inline">
									<input class="form-check-input" type="radio" name="bcrash" id="inlineRadioCrash" value="1">
									<label class="form-check-label" for="inlineRadioCrash">있음</label>
								</div>

							</div>
							</div>
						</div>	

						<div class="row mt-2 text-bg-secondary input-group-text"  id = "crash_option">

							<div class="col-md-5 offset-md-2">
			              	<div class="input-group-lg " >
			              	<label class="mx-1 text-light text-opacity-50" id="crash_text">사고유형:&nbsp; &nbsp;</label >

								<div class="form-check form-check-inline">
									<input class="form-check-input dr_checkbox" type="checkbox" name="bcrashobject" id="inlineCheckCrashObject" value="1" disabled required>
									<label class="form-check-label" for="inlineCheckCrashObject">대물</label>
								</div>
								<div class="form-check form-check-inline">
									<input class="form-check-input dr_checkbox" type="checkbox" name="bcrashhuman" id="inlineCheckCrashHuman" value="1" disabled required>
									<label class="form-check-label" for="inlineCheckCrashHuman">대인</label>
								</div>
								<div class="form-check form-check-inline dr_checkbox">
									<input class="form-check-input dr_checkbox" type="checkbox" name="bcrashonly" id="inlineCheckCrashOnly" value="1" disabled required>
									<label class="form-check-label" for="inlineCheckCrashOnly">단독</label>
								</div>

							</div>
							</div>
							<div class="col-md-6">
							</div>
						</div>	
						<br>
						<div class="row mt-2">
							<div class="col-md-3"></div>
							<div class="col-md-3">
								<button class="btn border-white text-white bg-black btn-outline-light" type="submit">등록</button>
							</div>
							<div class="col-md-3">
	                        	<button class="btn btn btn-outline-danger" onclick ="CancelAddUser()" type="button">취소</button>
							</div>
							
							<div class="col-md-3"></div>
							
						</div>
                        <br>
					</form>
					
					
                
                </div>
            
        </div>
    </section>
  
  
</div>		

<%
if( request.getParameter("myresult") != null )
{
	if( request.getParameter("myresult").equals("1") )
	{		
		if( request.getParameter("name") != null)
		{
%>
		<script>MessageBox( '<%=request.getParameter("name")%>' + ' 수검자 생성 성공');</script>
<%		
		}

	}
}
%>
<br>		
<br>
<footer class="mt-auto text-black-50">
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-black navbar-brand">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>

<script src="js/dr_join.js"></script>

</body>


</html>
