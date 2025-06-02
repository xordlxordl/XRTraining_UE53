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
	<link rel="stylesheet" href="css/my-floating-labels.css">
	
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/dark.css">
	<link rel="stylesheet" href="css/style.css">
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.js"></script>
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
		
		String uid = null;
		if( request.getParameter("uid") != null )
			uid = ((String)request.getParameter("uid"));
		

		arDBWork db = new arDBWork();
		arAdmin admin = db.spGetTrainerData( uid );
		
		if( admin == null)
		{
			
			%>
			<script>MyError('DB 오류');</script>
			<%
		}
		
	%>
	
<header class="navbar navbar-expand-md navbar-dark bg-dark" aria-label="Dark offcanvas navbar">
	<div class="container-fluid">
		<a class="navbar-brand" href="index.jsp"> <p class="fs-1">관제 계정 정보 수정</p> </a>
		
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
		
		<%
		if( iAdminGrade < 2 )
        {
      	%>
      	<hr>
      	<li class="nav-item"><a href="ar_trainer_add.jsp" class="nav-link text-white fs-4">계정 관리</a></li>
      	
      	<li class="nav-item"><a href="ar_trainer_add.jsp" class="nav-link ">관제 계정 추가</a></li>
      	<li class="nav-item"><a href="ar_trainer_list.jsp" class="nav-link text-white active" >관제 정보 수정</a></li>
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
                    <h1>관제 계정 정보 수정 페이지</h1>
                </div>
            </div>
            <div class="row d-flex justify-content-center">
                <div class="col-md-4">
                    <div class="card text-bg-light">
                    	<div class="card-header"> <h4>관제 계정 정보 수정</h3></div>
                    	
                        <div class="card-body d-flex flex-column align-items-center">
                            
                            
                            <form class="text-center" method="post" action="ar_trainer_edit_action.jsp">

                            	<div class="floating-label hide">
	                            	<input class="form-control  text-center text-white bg-dark"
	                            	data-bs-toggle="tooltip"
           							data-bs-placement="right"
           							data-bs-custom-class="custom-tooltip-d"
        							data-bs-title="수정 할 수 없습니다" 
	                            	type="text" name="uid" value="<%=admin.getUid()%>" readonly >
	                            	<label>번호</label>
                            	</div>
                            	
                            	
                            	
                            	<div class="input-group mb-1">
                            		<span class="input-group-text col-md-5">이름</span>
                            		<input class="form-control text-center text-white bg-dark"
                            		data-bs-toggle="tooltip"
           							data-bs-placement="right"
           							data-bs-custom-class="custom-tooltip-d"
        							data-bs-title="수정 할 수 없습니다" 
                            		type="text" name="name" id="id_trainer_name" value="<%=admin.getName()%>" readonly>
                            		
                            	</div>
                            	
                            	                            	
                                <div class="input-group mb-1">
                                	<span class="input-group-text col-md-5">군번</span>
                                	<input class="form-control text-center text-white bg-dark" id="id_num"
                                	data-bs-toggle="tooltip"
           							data-bs-placement="right"
           							data-bs-custom-class="custom-tooltip-d"
        							data-bs-title="수정 할 수 없습니다" 
                                	type="text" name="num" value="<%=admin.getNum()%>" readonly>
                                	
                                </div>
                                
                                <div class="input-group mb-1">
                                	<span class="input-group-text col-md-5">소속</span>
                                	<input class="form-control text-center"
                                	data-bs-toggle="tooltip"
           							data-bs-placement="right"
           							data-bs-custom-class="custom-tooltip-p"
        							data-bs-title="소속을 수정 합니다." 
                                	type="text" name="group" placeholder="" value="<%=admin.getGroup()%>" required>

                                </div>
                                
                                <div class="input-group mb-1">
                                	<span class="input-group-text col-md-5">계정</span>
                                	<input class="form-control text-center text-white bg-dark"
                                	data-bs-toggle="tooltip"
           							data-bs-placement="right"
           							data-bs-custom-class="custom-tooltip-d"
        							data-bs-title="수정 할 수 없습니다" 
                                	type="text" name="num" placeholder="계정" value="<%=admin.getAccount()%>" readonly>
                                	
                                </div>
                                
                                <div class="input-group mb-1">
                                	<span class="input-group-text col-md-5">신규 비밀번호</span>
                                	<input class="form-control"
                                	data-bs-toggle="tooltip"
           							data-bs-placement="right"
        							data-bs-title="입력하지 않으면 수정 되지 않습니다" 
                                	type="password" name="pass" placeholder="" onkeyup="OnChangePass()" maxlength="15" id="pass1">
                                	
                                </div>
                                
                                <div class="input-group mb-3" id="id_passdiv">
                                	<span class="input-group-text col-md-5">비밀번호 확인</span>
                                	<input class="form-control"
                                	data-bs-toggle="tooltip"
           							data-bs-placement="right"
        							data-bs-title="비밀번호 수정시 재확인"
                                	type="password" onkeyup="MatchCheckPassEdit()" placeholder="" maxlength="15" id="pass2" disabled>
                                	
                                </div>
                                
                                <div class="mb-2 mismatch-message hide fs-7 badge rounded-pill text-bg-danger">비밀번호가 일치하지 않습니다</div>
                                
                                <div class="bd-example">
                                <button class="btn btn-light btn-outline-dark col-md-4 btn-sm" onclick = "CheckPass()" type="button">저장하기</button>
                                
                                </div>
                            </form>
                            
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
  
</div>		

		
			
<footer class="mt-auto text-white-50 fixed-bottom">
	<%@ include file="foot.jspf" %>
</footer>

</body>
</html>
