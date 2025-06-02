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
      
		<li class="nav-item"><a href="it_userprod_main.jsp" class="nav-link text-white ">제품 이용 목록</a></li>
		<li class="nav-item"><a href="it_add_userprod.jsp" class="nav-link text-white fs-5 active">유저 제품 등록</a></li>
		
      
    </ul>
  </div>
  <!-- /사이드바 -->


  
  
	<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>유저 제품 등록 페이지</h1>
                </div>
            </div>
            <div class="row d-flex justify-content-center">
                <div class="col-md-6 col-xl-4">
                    <div class="card my-5 text-bg-light">
                    	<div class="card-header"> <h3>유저 제품 등록</h3></div>
                        <div class="card-body d-flex flex-column align-items-center">
                            <div class="my-3"></div>
                            
                            <form class="text-center" method="post" action="it_AddUserProdAction.jsp">
                                
                                
                                <div class="floating-label mb-4">
								<select id="product" class="form-select" name="uid" required>
							  	<option value="">유저 계정 목록</option>
							  	<%
							  	int i;
							  	ITDBWork db1 = new ITDBWork();
								ArrayList<ITUser> ITUserlist = db1.GetUserList();
								
								for( i = 0; i < ITUserlist.size(); ++i )
								{
							  	%>
							  	<option value=<%=ITUserlist.get(i).getUid()%>> 
							  	<%=ITUserlist.get(i).getLoginid()%></option>
							  	<%
								}
							  	%>
								</select>
								</div>
                                
                                <div class="floating-label mb-4">
								<select id="product" class="form-select" name="pid" required>
							  	<option value="">제품 목록</option>
							  	<%
							  	ITDBWork db = new ITDBWork();
								ArrayList<ITProd> ITProdlist = db.GetProdList();
								
								for( i = 0; i < ITProdlist.size(); ++i )
								{
							  	%>
							  	<option value=<%=ITProdlist.get(i).getPid()%>> 
							  	<%=ITProdlist.get(i).getName()%></option>
							  	<%
								}
							  	%>
								</select>
								</div>
                                
                                
                              
                                
                                <div class="floating-label mb-4" id="id_startdate"
                                data-bs-toggle="tooltip" 
                                data-bs-placement="top" 
        						data-bs-custom-class="custom-tooltip"
        						data-bs-title="미 입력시 NOW()"
                                
                                >
                                <input class="form-control" type="datetime-local" step=1 name="createat" placeholder="종료 날짜">
                                <label for="id_startdate">시작 날짜</label>
                                </div>
                                
                                <div class="floating-label mb-3" id="id_expireddate"
                                data-bs-toggle="tooltip" 
                                data-bs-placement="top" 
        						data-bs-custom-class="custom-tooltip"
        						data-bs-title="입력하지 않아도 OK"
                                
                                >
                                <input class="form-control" type="datetime-local" step=1 name="expiredat" placeholder="종료 날짜">
                                <label for="id_expireddate">종료 날짜</label>
                                </div>
                                
                                
                                <div class="floating-label mb-4" id=id_memo>
                                <input class="form-control" type="text" name="memo" placeholder="비고">
                                <label for="id_memo">비고</label>
                                </div>
                                
                                
                                <div class="bd-example">
                                <button class="btn border-white text-white bg-primary btn-outline-light" type="submit">등록</button>
                                <button class="btn btn btn-outline-danger" onclick ="CancelAddProd()" type="button">취소</button>
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
		<script>MessageBox( '<%=request.getParameter("name")%>' + '유저 제품 생성 성공');</script>
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
