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
	<link rel="stylesheet" href="css/jquery.timepicker.min.css">

	
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/jquery.timepicker.min.js"></script>

	
	<script src="js/my.js"></script>
	
	<title>INTERACT SYTEM</title>
</head>

	<%

		String pid = null;
		if( request.getParameter("uid") != null )
			pid = ((String)request.getParameter("uid"));
		
		
		ITUserProductData userprod = new ITUserProductData();
		ITDBWork db1 = new ITDBWork();
		userprod = db1.GetUserProdToIndex( pid );
		
		
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
			<div class="offcanvas-header">
				
				<button type="button" class="btn-close btn-close-white" data-bs-dismiss="offcanvas" aria-label="Close"></button>
			</div>
				
			<div class="offcanvas-body">
				<ul class="navbar-nav nav-masthead justify-content-end flex-grow-1">
					<li class="nav-item active">
				
					</li>
					
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
		<li class="nav-item"><a href="it_userprod_main.jsp" class="nav-link text-white">제품 이용 목록</a></li>
      <hr>
    </ul>
  </div>
  <!-- /사이드바 -->


  
  
	<section>
		<div class="container">
			<div class="row mb-5">
				
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>유저 제품 수정 페이지</h1>
                </div>
            </div>
            <div class="row d-flex justify-content-center">
                <div class="col-md-8 col-xl-4 text-center mx-auto">
                    <div class="card my-5 text-bg-light text-center">
                    	<div class="card-header"> <h3>유저 제품 수정</h3></div>
                        <div class="card-body  d-flex flex-column align-items-center">
                            
                            
                            <form class="text-center" method="post" action="it_UpdateUserProdAction2.jsp">
                                
                                <div class="floating-label mb-4" id=id_puid>
                                <input class="form-control text-white bg-black text-center" type="number" id="floatingIndex" name="puid" placeholder="PID" maxlength="15" readonly 
                                value="<%=userprod.getPuid()%>"> 
                                <label for="id_puid">Index</label>
                                </div>
                                
                                
                                <div class="floating-label mb-4" id=id_userid>
                                <input class="form-control text-white bg-black text-center" type="text" name="uid" placeholder="유저 ID" maxlength="15" readonly
                                value='<%=userprod.getUid() %>'>
                                <label for="id_userid">유저ID</label>
                                </div>
                                
                                
                                <div class="floating-label mb-4" id=id_prodname>
								<select id="product" class="form-select" name="pid" required>
							  	<option value="">제품 이름</option>
							  	<%
							  	ITDBWork db = new ITDBWork();
								ArrayList<ITProd> list = db.GetProdList();
								int i;
								for( i = 0; i < list.size(); ++i )
								{
							  	%>
							  	<option value=<%=list.get(i).getPid()%> 
							  	<%
							  	if( userprod.getPid() == list.get(i).getPid() )
							  	{
							  	%>
							  	selected
							  	<%
							  	}
							  	%> 
							  	> 
							  	<%=list.get(i).getName()%></option>
							  	<%
								}
							  	%>
								
								</select>
								<label for="id_prodname">제품 이름</label>
								</div>

								
								

                                <div class="floating-label mb-4">
                                <input class="form-control" type="datetime-local" step=1 name="createat" id ="id_startdate" placeholder="시작 날짜" 
                                value='<%=userprod.getCreateat() %>'>
                                <label for="id_startdate">시작 날짜</label>
                                </div>
                                
                                <div class="floating-label mb-3">
                                <input class="form-control" type="datetime-local" step=1 name="expiredat" placeholder="종료 날짜" 
                                value='<%=userprod.getExpiredat() %>'>
                                <label for="id_expireddate">종료 날짜</label>
                                </div>
                                
                                
                                
                                
                                <div class="mb-3"><input class="form-control text-center" type="text" name="memo" placeholder="비 고" 
                                value='<%=userprod.getMemo() %>'></div>
                                
                                <div class="bd-example">
                                <button class="btn border-white text-white bg-primary btn-outline-light" type="submit">수정</button>
                                <button class="btn btn btn-outline-danger" onclick ="CancelAddProd()" type="button">취소</button>
                                </div>
                            </form>
                            
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
