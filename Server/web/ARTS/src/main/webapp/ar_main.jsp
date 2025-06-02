<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="data.*" %>
<%@ page import="db.*" %>
<%@ page import="java.util.ArrayList"%>
<%@ page import="java.io.PrintWriter" %>
<%@ page import="org.json.simple.JSONArray" %>
<%@ page import="org.json.simple.JSONObject" %>

<%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
   
<!DOCTYPE html>
<html class="h-100">

<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	<link rel="stylesheet" href="css/MyChartDataTables.css">
	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<!--  <link rel="stylesheet" href="css/cover.css">-->
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/MyChartTableStyle.css">
	
	
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>

	<script src="js/chart-utils.min.js"></script>
    <script src="js/chart.js"></script>
    <script src="js/chartjs-plugin-datalabels.js"></script>
    
    <script src="js/jquery-3.6.4.min.js"></script>
    <script src="js/jquery.dataTables.js"></script>
    <script src="js/dataTables.bootstrap5.js"></script>
    <script src="js/full_numbers_no_ellipses.js"></script>
    
    <script src="js/myTableChart1.js"></script>
    <script src="js/myTableChart2.js"></script>
    
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
		
		arDBWork db = new arDBWork();
		JSONArray jsonArray = db.webGetMonthUserData();
		request.setAttribute("DataArray", jsonArray);
		
		arDBWork db1 = new arDBWork();
		JSONArray TrainPerArray = db1.webGetTrainingPer();
		request.setAttribute("TrainPerArray", TrainPerArray);
		
		arDBWork db2 = new arDBWork();
		JSONArray TrainCountArray = db2.webGetTrainingCount();
		request.setAttribute("TrainCountArray", TrainCountArray);
		
		arDBWork db3 = new arDBWork();
		JSONArray TrainTimeArray = db3.webGetTrainingTime();
		request.setAttribute("TrainTimeArray", TrainTimeArray);
		
		
		
	%>
	

<script type="text/javascript">
var jsonData = ${DataArray};
var TrainPerData = ${TrainPerArray};
var TrainCountArray = ${TrainCountArray};
var TrainTimeArray = ${TrainTimeArray};
</script>


<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="#"> <p class="fs-1">교육 훈련 관리 메인 화면</p> </a>
		
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
						<a class = "nav-link active" aria-current="page" href="ar_main.jsp">메인</a>
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
							<a class="nav-link" href="ar_logoutAction.jsp">로그아웃</a>
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

<div id="page-wrapper">
  <!-- 사이드바 -->
  
  
  
  <div id="sidebar-wrapper" class="flex-column flex-shrink-0 p-3 text-bg-dark">

  
 
    <ul class="sidebar-nav nav nav-masthead flex-column mb-auto">
    
	<%
	if( iAdminGrade < 2 )
    {
	%> 
      <li class="sidebar-brand">
        <a href="ar_main1.jsp" class="align-items-center mb-3 mb-md-0 me-md-auto text-white text-decoration-none">
          <span class="fs-3">시스템</span>
        </a>
      </li>
    </ul>
    <hr>
	<%
    }
	%>

	
    <ul class="sidebar-nav nav nav-masthead flex-column mb-auto">
  
      	<li class="nav-item"><a href="ar_user_add.jsp" class="nav-link text-white fs-4">인원 관리</a></li>
      	<hr>
		<li class="nav-item"><a href="tr_list.jsp" class="nav-link text-white fs-4">교육 관리</a></li>
		<hr>
		<li class="nav-item"><a href="ar_mch_list.jsp" class="nav-link text-white fs-4">장비 관리</a></li>
		<hr>
		<%
		if( iAdminGrade < 2 )
        {
      	%>
      	<li class="nav-item"><a href="ar_trainer_add.jsp" class="nav-link text-white fs-4">계정 관리</a></li>
      	<%
        }
      	%>
    </ul>
  </div>
  <!-- /사이드바 -->
  	  <div class="container">
        <div class="row">
            <div class="col-md-5">
                <canvas id="mychart4"></canvas>
            </div>
            <div class="col-md-1"></div>
            <div class="col-md-3">
                <canvas id="mychart1"></canvas>
            </div>
        </div>
      </div>
<br>
<br>
<div class="container">
        <div class="row">
            <div class="col-md-5">
                <canvas id="mychart3"></canvas>
            </div>
            	<div class="col-md-1"></div>
            <div class="col-md-5">
                <canvas id="mychart2"></canvas>
            </div>
        </div>
    </div>
    <br>
    <br>
    <div class="container">
        <div class="row">
        <div id="mychart5" class="col-5">
        	<h4>최근 수행된 훈련</h4>
	            	<table id="id_maintable1" class="table table-striped table-bordered table-dark table-hover compact" style="table-layout:fixed ; border:1px solid #fff">
					<thead>
						<tr>
							<!-- <th class="bg-white col-md-1 text-black">번호</th>  -->
							<th class="bg-white col-md-3 text-black">시작 시간</th>
							<!-- <th class="bg-white col-md-3 text-black">종료 시간</th>  -->
							<th class="bg-white col-md-2 text-black">소요 시간</th>
							<th class="bg-white col-md-3 text-black">훈련 시나리오</th>
							<th class="bg-white col-md-2 text-black">상세</th>
						</tr>
					</thead>
					<tbody id="id_maintbody1" style="display:none">
							<%
							arDBWork db4 = new arDBWork();
																
							ArrayList<arTrData>	list = db4.webGetTrDataListDesc();
							
							for(int i = 0; i < list.size(); ++i )
							{
							%>
						<tr>
							<!-- <td><%= list.get(i).getTID() %> </td>  -->
							<td><%= list.get(i).getStartTime() %></td>
							<!-- <td><%= list.get(i).getEndTime() %></td> -->
							<td><%= list.get(i).getPlayTime() %></td>
							<td><%= list.get(i).getTrName() %></td>
							<td> 
								<button 
									type="button" 
									class="btn btn-outline-primary col-md-6 btn-ssm text-white" 
									style="cursor: pointer;" 
									onclick="GoToTRUserList(<%=list.get(i).getTID()%>);">보기</button></td>
						</tr>
							<%
							}
							%>
					</tbody>
				</table>
            </div>
            <div class="col-1"></div>
            
            	<div id="mychart6" class="col-5">
            	<h4>최근 업데이트 된 장비</h4>
	            	<table id="id_maintable2" class="table table-striped table-bordered table-dark table-hover compact" style="table-layout:fixed ; border:1px solid #fff">
				<thead>
					<tr>
						<!-- <th class="bg-white col-md-1 text-black col-sm-1">nID</th> -->
						<th class="bg-white col-md-1 text-black">장비그룹명</th>
						<th class="bg-white col-md-1 text-black">백팩PC</th>
						<th class="bg-white col-md-1 text-black">HMD</th>
						<th class="bg-white col-md-1 text-black">총기트래커</th>
						<th class="bg-white col-md-1 text-black">수정</th>
					</tr>
				</thead>
				<tbody id="id_maintbody2" style="display:none">
						<%
						arDBWork db5 = new arDBWork();
															
						ArrayList< arMachine > list2 = db5.webGetMCList();
						
						for(int i = 0; i < list2.size(); ++i )
						{
						%>
					<tr>
						<!-- <td><%= list2.get(i).getnID() %> </td> -->
						<td><%= list2.get(i).getName() %> </td>
						<td><%= list2.get(i).getBPC() %> </td>
						<td><%= list2.get(i).getHMD() %> </td>
						<td><%= list2.get(i).getUTT1() %> </td>

						<td> <button type="button" class="btn btn-outline-primary col-md-6 btn-ssm text-white" style="cursor: pointer;" onclick="GoToMCHEditWithIndex(<%=list2.get(i).getnID()%>);">수정</button></td>
					</tr>
						<%
						}
						%>
				</tbody>
			</table>
            </div>
        </div>
       </div> 
  
<br>		
<br>
<footer class="mt-auto text-white-50 ">
	<%@ include file="foot.jspf" %>
</footer>
	<script src="js/bootstrap.min.js"></script>
    
    <script src="js/bs-init.js"></script>
    <script src="js/theme.js"></script>
    
    <script src="js/myChart1.js"></script>
	<script src="js/myChart2.js"></script>
	<script src="js/myChart3.js"></script>
	<script src="js/myChart4.js"></script>
	
</body>
</html>
