<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>

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
	<script src="js/bootstrap.bundle.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/my.js"></script>
	
	
	<title>XR TRANING SYTEM</title>
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
		
		String nid = null;
		if( request.getParameter("nid") != null )
			nid = ((String)request.getParameter("nid"));
		
		String mapid = null;
		if( request.getParameter("mapid") != null )
			mapid = (String) request.getParameter("mapid");
		
		String name = null;
		if( request.getParameter("name") != null )
			name = (String) request.getParameter("name");

		String ip = null;
		if( request.getParameter("ip") != null )
			ip = (String) request.getParameter("ip");
		
		String port = null;
		if( request.getParameter("port") != null )
			port = (String) request.getParameter("port");
		
	%>
	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1"> XR Traninig System</p> </a>
		
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
						<a class = "nav-link active" aria-current="page" href="main.jsp">메인</a>
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
							<a class="nav-link" href="logoutAction.jsp">관리자 로그아웃</a>
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
      <li class="sidebar-brand">
        <a href="main.jsp" class="align-items-center mb-3 mb-md-0 me-md-auto text-white text-decoration-none">
        	<span class="fs-3">관리메뉴</span>
        </a>
      </li>
      <hr>
      <%
      	if( iAdminGrade < 2 )
      	{
      %>
		<li class="nav-item"><a href="xr_main.jsp" class="nav-link text-gray">XR기기</a></li>
		<li class="nav-item"><a href="server_main.jsp" class="nav-link text-white">서버</a></li>
	    <li class="nav-item"><a href="map_main.jsp" class="nav-link text-gray">시나리오</a></li>
	    <li class="nav-item"><a href="training_main.jsp" class="nav-link text-gray">훈련</a></li>
	 <%
      	}
	 %>
      <hr>
    </ul>
  </div>
  <!-- /사이드바 -->
  
	<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>서버 상세 페이지</h1>
                </div>
            </div>
            <div class="row d-flex justify-content-center">
                <div class="col-md-6 col-xl-4">
                    <div class="card my-5 text-bg-light ">
                    	<div class="card-header"> <h3>서버 내용</h3></div>
                        <div class="card-body d-flex flex-column align-items-center">
                            <div class="my-3"></div>
                            
                            <form class="text-center" method="post" action="EditServerAction.jsp">
                            	<div class="mb-3"><input class="form-control text-white bg-black" type="number" name="nid" placeholder="서버ID" maxlength="15" readonly value="<%=nid%>"></div>
                                
                                
                                <div class="form-floating mb-3">
                                	<input class="form-control" id="id_myinput" type="text" name="mapid" placeholder="시나리오(맵) ID" maxlength="15" value="<%=mapid%>">
                                	 <label for="id_myinput">시나리오(맵) ID</label>
                                </div>
                                
                                <div class="mb-3"><input class="form-control" type="text" name="name" placeholder="이름" maxlength="15" value="<%=name%>"></div>
                                <div class="mb-3"><input class="form-control"data-inputmask="'alias': 'ip'" inputmode="numeric" id="ipAddress" name="ip" placeholder="서버주소 (IP정" maxlength="15" value="<%=ip%>"></div>
                                <div class="mb-3"><input class="form-control" type="number" name="port" placeholder="포트번호" maxlength="15" value="<%=port%>"></div>
                                <div class="bd-example">
                                <button class="btn border-white text-white bg-black btn-outline-light" type="submit">수정</button>
                                
                                
                            </form>
                            <a onclick="DelServer('<%=nid%>');" class="btn btn-outline-danger"  type="button">삭제</a>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
  
  
</div>		

<footer class="mt-auto text-white-50 fixed-bottom">
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>

<script src="js/prism.js"></script>
<script type="text/javascript" src="js/jquery.inputmask.min.js" charset="utf-8"></script>
<script type="text/javascript" src="js/inputmask.js" charset="utf-8"></script>
<script type="text/javascript" src="js/my-input-mask.js" charset="utf-8"></script>

</body>
</html>
