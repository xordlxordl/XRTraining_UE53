<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>

<%@ page import="data.*" %>
<%@ page import="db.*" %>
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
	<link rel="stylesheet" href="css/jquery-ui.css">
	<link rel="stylesheet" href="css/style.css">
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/jquery.dataTables.js"></script>
	<script src="js/dataTables.bootstrap5.js"></script>
	<script src="js/full_numbers_no_ellipses.js"></script>
	
	<script src="js/jquery-ui.js"></script>
	<script src="js/jquery.ui-contextmenu.js"></script>
	<script src="js/myTableTrainer.js"></script>


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
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> 관제 계정 목록</p> </a>
		
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
  
      	<li class="nav-item"><a href="ar_user_add.jsp" class="nav-link fs-4">인원 관리</a></li>
      	<hr>
		<li class="nav-item"><a href="tr_list.jsp" class="nav-link fs-4">교육 관리</a></li>
		<hr>
		<li class="nav-item"><a href="ar_mch_list.jsp" class="nav-link fs-4">장비 관리</a></li>
		<hr>
		<%
      	if( iAdminGrade < 2 )
        {
      	%>
      	<li class="nav-item"><a href="ar_trainer_add.jsp" class="nav-link text-white fs-4">계정 관리</a></li>
      	<li class="nav-item"><a href="ar_trainer_add.jsp" class="nav-link ">관제 계정 추가</a></li>
      	<li class="nav-item"><a href="ar_trainer_list.jsp" class="nav-link fs-6 active">관제 정보 수정</a></li>
      	<%
        }
      	%>
		
    </ul>
  </div>
  <!-- /사이드바 -->

	<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>관제 계정 목록</h1>
                </div>
            </div>
            
			<table id="id_maintable" class="table table-striped table-bordered table-dark table-hover compact" style="table-layout:fixed ; border:1px solid #fff">
				<thead>
					<tr>
						<th class="bg-white text-black col-md-1">UID</th>
						<th class="bg-white text-black col-md-2">관제자명</th>
						<th class="bg-white text-black col-md-2">군번</th>
						<th class="bg-white text-black col-sm-3">소속</th>
						<th class="bg-white text-black">계정</th>
						<th class="bg-white text-black col-md-2">수정</th>
					</tr>
				</thead>
				<tbody id="id_maintbody" style="display:none">
						<%
						arDBWork db = new arDBWork();
															
						ArrayList<arAdmin>	list = db.spGetTrainerList();
						int i;
						for( i = 0; i < list.size(); ++i )
						{
						%>
					<tr>
						<td><%= list.get(i).getUid() %> </td>
						<td><%= list.get(i).getName() %> </td>
						<td><%= list.get(i).getNum() %> </td>
						<td class="text-start ps-4"><%= list.get(i).getGroup() %> </td>
						<td><%= list.get(i).getAccount() %></td>
						<td> <button type="button" class="btn btn-outline-primary col-md-6 btn-ssm text-white" style="cursor: pointer;" onclick="GoToTrainerEditWithIndex(<%=list.get(i).getUid()%>);">수정하기</button></td>
					</tr>
						<%
						}
						%>
				</tbody>
			</table>
		</div>
    </section>
  
  
</div>		


<footer class="mt-auto text-white-50 fixed-bottom">
	<%@ include file="foot.jspf" %>
</footer>

</body>


</html>
