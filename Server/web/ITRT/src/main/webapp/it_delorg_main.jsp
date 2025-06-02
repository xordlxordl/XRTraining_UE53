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
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/jquery.dataTables.js"></script>
	<script src="js/dataTables.bootstrap5.js"></script>
	<script src="js/full_numbers_no_ellipses.js"></script>
	
	<script src="js/jquery-ui.js"></script>
	<script src="js/jquery.ui-contextmenu.js"></script>
	<script src="js/my.js"></script>
	<script src="js/it_myTable4.js"></script>


	<title>INTERACT SYTEM</title>
</head>

	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> INTERACT SYTEM</p> </a>
		
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

<div id="page-wrapper">
  <!-- 사이드바 -->
  <div id="sidebar-wrapper" class="flex-column flex-shrink-0 p-3 text-bg-dark">
  
    <ul class="sidebar-nav nav nav-masthead flex-column mb-auto">
    
      <li class="sidebar-brand">
        <a href="index.jsp" class="align-items-center mb-3 mb-md-0 me-md-auto text-white text-decoration-none">
        	<span class="fs-3"
        	data-bs-toggle="tooltip"
			data-bs-placement="top"
			data-bs-custom-class="custom-tooltip"
			data-bs-title="메인 페이지로 돌아갑니다"        	
        	>메인 메뉴</span>
        </a>
      </li>
      
		<hr>
      
		<li class="nav-item"><a href="it_org_main.jsp" class="nav-link text-white">조직</a></li>
		<li class="nav-item"><a href="it_add_org.jsp" class="nav-link text-white">등록</a></li>
		<li class="nav-item"><a href="#" class="nav-link text-white fs-4 active">삭제 된 조직</a></li>
		
      <hr>
      <span class="fs-3"
      data-bs-toggle="tooltip"
      data-bs-placement="top"
      data-bs-custom-class="custom-tooltip"
      data-bs-title="복구 할 조직을 선택하세요"
      >복구메뉴</span>
      	<li class="nav-item" id="id_modyfymenu" style="display:none"><a href="#" onclick="RepairOrg()" class="btn btn-primary" type="button" >복구</a></li>

    </ul>
  </div>
  <!-- /사이드바 -->

	<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>삭제된 조직 리스트</h1>
                </div>
            </div>
			<table id="id_maintable" class="table table-striped table-bordered table-dark table-hover" style="table-layout:fixed ; border:1px solid #fff">
					<thead>
					<tr>
						<th class="bg-white text-black text-center" >조직 ID</th>
						<th class="bg-white text-black text-center" >조직 타입</th>
						<th class="bg-white text-black text-center" >조직 이름</th>
						<th class="bg-white text-black text-center" >비고</th>
					</tr>
				</thead>
				<tbody id="id_maintbody" style="display:none">
						<%
						ITDBWork db = new ITDBWork();
						ArrayList< ITOrg > list = db.GetDelOrgList();
						int i;
						for( i = 0; i < list.size(); ++i )
						{
						%>

					<tr>
						<td class="text-center"><%= list.get(i).getOid() %></td>
						<td class="text-center"><%= list.get(i).getTypename() %></td>
						<td class="text-center"><%= list.get(i).getName() %></td>
						<td class="text-center"><%= list.get(i).getMemo() %></td>
						
						
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
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white navbar-brand">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>



</body>


</html>
