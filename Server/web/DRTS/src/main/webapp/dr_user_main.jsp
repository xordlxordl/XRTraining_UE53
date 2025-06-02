<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>

<%@ page import="data.DRAdmin" %>
<%@ page import="data.DRUser" %>
<%@ page import="db.DRDBWork" %>

<%@ page import="java.util.ArrayList"%>
<%@ page import="java.io.PrintWriter" %>

<%
request.setCharacterEncoding("UTF-8");
%>


<!DOCTYPE html>
<html class="h-100">

<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	
	<link rel="stylesheet" href="css/MyDataTables.css">
	<link rel="stylesheet" href="css/bootstrap.css">
	
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/style.css">
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	<script src="js/bootstrap.bundle.min.js"></script>
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/jquery.dataTables.js"></script>
	<script src="js/dataTables.bootstrap5.js"></script>
	<script src="js/full_numbers_no_ellipses.js"></script>
	<script src="js/myTable.js"></script>
	<script src="js/my.js"></script>
	<script src="js/Calibration.js"></script>
	<script src="js/DRServer.js"></script>
	
	<title>DR TRANING SYTEM</title>
<style>
.custom-tooltip {
  --bs-tooltip-bg: var(--bs-primary);
}
</style>

</head>


<%
	String strAdminID = null;

	
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
	

  

<body class="h-75 text-bg-dark">

<div id="page-wrapper">
  <!-- 사이드바 -->
  <div id="sidebar-wrapper" class="flex-column flex-shrink-0 p-3 text-bg-dark text-center">
  
    <ul class="sidebar-nav nav nav-masthead flex-column mb-auto text-center">
      <li class="sidebar-brand">
        <a class="align-items-center mb-3 mb-md-0 me-md-auto text-white text-decoration-none">
        	<span class="fs-3">관리 메뉴</span>
        </a>
      </li>
      <hr>
      
	      <li class="nav-item"><a href="dr_user_main.jsp" class="nav-link  text-white fs-4 active">수검자</a></li>
			<li class="nav-item">
				<a class="nav-link text-white" href="dr_join_user.jsp">등록</a>
			</li>
	      
      <hr>
        <span class="fs-3" class="text-decoration-none" data-bs-toggle="tooltip" data-bs-placement="top" 
        data-bs-custom-class="custom-tooltip"
        data-bs-title="수정 할 대상을 목록에서 선택하세요.">수정 메뉴</span>
      	<li class="nav-item" id="id_modyfymenu" style="display:none"><a href="#" onclick="GoToUserView()" class="nav-link text-white">수정</a></li>
      	<li class="nav-item" id="id_delmenu" style="display:none"><a onclick="DelUser();" class="btn btn-danger" type="button">삭제</a></li>
      <hr>
      <span class="fs-3" class="text-decoration-none" data-bs-toggle="tooltip" data-bs-placement="top" 
        data-bs-custom-class="custom-tooltip"
        data-bs-title="운전 대상을 목록에서 선택하세요.">운전 메뉴</span>
      	<li class="nav-item" id="id_simmenu" style="display:none"><a onclick="GoToSimView();" class="btn btn-primary" type="button">시뮬레이션</a></li>
    </ul>
  </div>
  <!-- /사이드바 -->


<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>수검자 목록</h1>
                    
                </div>
            </div>
            
			<table id="id_maintable" class="table table-striped table-bordered table-dark table-hover" style="table-layout:fixed ; border:1px solid #fff" width="100%">
				<thead>
					<tr>
						<th class="bg-white text-black text-center" >UID</th>
						<th class="bg-white text-black text-center" >이름</th>
						<th class="bg-white text-black text-center" >생일</th>
						<th class="bg-white text-black text-center" >나이</th>
						<th class="bg-white text-black text-center" >성별</th>
						<th class="bg-white text-black text-center col-md-3" >거주지역</th>
						<th class="bg-white text-black text-center" >경력</th>

					</tr>
				</thead>
				<tbody id="id_maintbody" style="display:none">
						<%
						DRDBWork db = new DRDBWork();
						ArrayList<DRUser>	list = db.GetTraineeList();
						int i;
						for( i = 0; i < list.size(); ++i )
						{
						%>

					<tr>
						<td class="text-center"><%= list.get(i).getUid() %></td>
						<td class="text-center"><%= list.get(i).getSzname() %></td>
						<td class="text-center"><%= list.get(i).getBirthdate() %></td>
						<td class="text-center"><%= list.get(i).GetAge() %></td>
						<td class="text-center"><%= list.get(i).getSzSex() %></td>
						<td class="text-start"><%= list.get(i).getSzaddr() %></td>
						<td class="text-end"><%= list.get(i).GetCareer() %></td>
						
						
					</tr>
						<%
						}
						%>
					
				</tbody>
			</table>
			
		</div>
</section>
	
  
		
<%
if( request.getParameter("myresult") != null )
{
	if( request.getParameter("myresult").equals("1") )
	{		
		if( request.getParameter("name") != null)
		{
%>
		<script>MessageBox( '<%=request.getParameter("name")%>' + ' 로그인');</script>
<%		
		}
		else
		{
%>
		<script>MessageBox('로그인');</script>
<%
		}

	}
	
}
%>
<script>

$(document).ready(function(){

  $('[data-bs-toggle="tooltip"]').tooltip();   

});
</script>

<br>		
<br>
<footer class="mt-auto text-white-50 text-center">
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white navbar-brand">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>



</body>
</html>
