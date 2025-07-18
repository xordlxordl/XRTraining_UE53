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
	
	<link rel="stylesheet" href="css/sidebars.css">
	
	<link rel="stylesheet" href="css/style.css">
	<link rel="stylesheet" href="css/fontawesome.all.css">
	<!--<link rel="stylesheet" href="css/datatables.css">-->
	
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	

	
	<script src="js/myTableUserDetail.js"></script>
	<script src="js/datatables.js"></script>
	


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
	
	String tid = null, uid = null;
	if( request.getParameter("tid") != null )
		tid = ((String)request.getParameter("tid"));
	
	if( request.getParameter("uid") != null )
		uid = ((String)request.getParameter("uid"));

	arDBWork db = new arDBWork();
	
	arLogDetail info = db.spGetUserLogDetail( tid, uid );
%>
	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<div class="navbar-brand"><div class="fs-1 col-md-2">교육 관리><div class="fs-2 col-md-1 d-inline">최근 훈련 보기</div></div></div>
		
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
		<li class="nav-item"><a href="tr_list.jsp" class="nav-link text-white fs-4">교육 관리</a></li>
		<li class="nav-item"><a href="tr_list.jsp" class="nav-link active">훈련 보기</a></li>
		<li class="nav-item"><a href="tr_user_list.jsp" class="nav-link ">훈련자 검색</a></li>
		<hr>
		<li class="nav-item"><a href="ar_mch_list.jsp" class="nav-link fs-4">장비 관리</a></li>
		<hr>
		<%
		if( iAdminGrade < 2 )
        {
      	%>
      	<li class="nav-item"><a href="ar_trainer_add.jsp" class="nav-link fs-4">계정 관리</a></li>
      	<%
        }
      	%>
    </ul>
  </div>
  <!-- /사이드바 -->

	<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-12 text-start mb-2">
                    <h4 id="id_h4"><%=info.getJob()%>&nbsp;<%=info.getMName()%>&nbsp;<%=info.getUName()%>&nbsp;<%=info.getNum()%></h4>
                </div>
                <div class="col-md-12 text-start">
                    <h3 id="id_h3"><%=info.getTName()%>&nbsp;<%=info.getnStartTime()%>(시작시간)</h3>
                </div>
            </div>
			<table id="id_maintable" class="table table-striped table-bordered table-dark table-hover compact" style="table-layout:fixed ; border:1px solid #fff">
			
				<thead>
					<tr>
						<th class="bg-white text-black text-center">훈련시간</th>
						<th class="bg-white text-black text-center">보직</th>
						<th class="bg-white text-black text-center">사격횟수</th>
						<th class="bg-white text-black text-center">적중횟수</th>
						<th class="bg-white text-black text-center">투항요구횟수</th>
						<th class="bg-white text-black text-center">포박횟수</th>
						<th class="bg-white text-black text-center">이동지시횟수</th>
						<th class="bg-white text-black text-center">증원요청횟수</th>
						<th class="bg-white text-black text-center">EOD요청횟수</th>
						<th class="bg-white text-black text-center">아군오사횟수</th>
						<th class="bg-white text-black text-center">이동거리</th>
						
					</tr>
				</thead>
				<tbody id="id_maintbody" style="display:none">
					<tr>
						<td><%= info.getPlayTime() %>회</td>
						<td><%= info.getJob()%>회</td>
						<td><%= info.getFireCount() %>회</td>
						<td><%= info.getHitCount() %>회</td>
						<td><%= info.getSurrenderCount()%>회</td>
						<td><%= info.getBindCount() %>회</td>
						<td><%= info.getCommandMoveCount() %>회</td>
						<td><%= info.getCallCount() %>회</td>
						<td><%= info.getEodCount() %>회</td>
						<td><%= info.getBadFireCount()%>회</td>
						<td>없음m</td>
					</tr>
						
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
