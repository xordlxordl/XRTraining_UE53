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
	
	<title>INTERACT SYTEM</title>
</head>

	<%

		String oid = null;
		if( request.getParameter("uid") != null )
			oid = ((String)request.getParameter("uid"));
		
		if( oid == null)
		{
			%>
			<script> UserMessageError("유저 정보를 불러오지 못했습니다."); </script>
			
			<%
			return;
		}
		
		ITOrg org = new ITOrg();
		ITDBWork db1 = new ITDBWork();
		org = db1.GetOrgData( oid );
		
		
	%>
	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1">INTERACT SYTEM</p> </a>
		
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
		<li class="nav-item"><a class="nav-link text-white" href="it_add_user.jsp">등록</a></li>
		
      <hr>
    </ul>
  </div>
  <!-- /사이드바 -->


  
  
	<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>조직 수정 페이지</h1>
                </div>
            </div>
            <div class="row d-flex justify-content-center">
                <div class="col-md-6 col-xl-4">
                    <div class="card my-5 text-bg-light">
                    	<div class="card-header"> <h3>조직 수정</h3></div>
                        <div class="card-body d-flex flex-column align-items-center">
                            <div class="my-3"></div>
                            
                            <form class="text-center" method="post" action="it_UpdateOrgAction.jsp">
                                
                                <div class="mb-3"><input class="form-control text-white bg-black" type="number" name="oid" placeholder="OID" maxlength="15" readonly value="<%=org.getOid()%>"></div>
								<div class="mb-3">
								<select id="orgoid" class="form-select" name = "type" required>
							  	<option value="">조직 타입</option>
							  	<option value=0 
							  	<% if( org.getType() == 0 )
							  	{
							  	%>
							  		selected
							  	<%
							  	}
							  	%> 
							  	>개인</option>
							  	<option value=1
							  	<% if( org.getType() == 1 )
							  	{
							  	%>
							  		selected
							  	<%
							  	}
							  	%>							  	 
							  	>회사</option>
							  	<option value=2 
							  	<% if( org.getType() == 2 )
							  	{
							  	%>
							  		selected
							  	<%
							  	}
							  	%>
							  	>공공기관</option>
								</select>
								</div>
							
                                
                                <div class="mb-3"><input class="form-control" type="text" name="name" placeholder="조직 이름" maxlength="15"
                                value=<%=org.getName() %>></div>
                                <div class="mb-3"><input class="form-control" type="text" name="memo" placeholder="비고" maxlength="15"
                                value=<%=org.getMemo() %>></div>
                                <div class="bd-example">
                                <button class="btn border-white text-white bg-primary btn-outline-light" type="submit">수정</button>
                                <button class="btn btn btn-outline-danger" onclick ="CancelAddOrg()" type="button">취소</button>
                                </div>
                            </form>
                            
                            <div id="liveAlertPlaceholder"></div>
                            <div class="my-2"></div>
                        </div>
                    </div>
                </div>
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
		<script>MessageBox( '<%=request.getParameter("name")%>' + ' 조직 생성 성공');</script>
<%		
		}

	}
}
%>
		
			
<footer class="mt-auto text-white-50 fixed-bottom">
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white navbar-brand">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>

</body>
</html>
