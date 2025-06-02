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
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
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
	
	DBWork db1 = new DBWork();
	Integer user_count = db1.GetAdminCount();
	
	MakePage mp = new MakePage();
	int pageno = 0;
	
	if( request.getParameter("pageno") != null )
		pageno = mp.toInt( request.getParameter("pageno") );
	
	mp.total_record = user_count;
	mp.Make( pageno );
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
					<li class="nav-item">
						<a class="nav-link" href="bbs.jsp">게시판</a>
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
					<form class="d-flex mt-3" role="search">
           				<input class="form-control me-2" type="search" placeholder="Search" aria-label="Search">
           				<button class="btn btn-secondary border-white text-white bg-black btn-outline-light w-50" type="submit">Search</button>
     				</form>
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
		<li class="nav-item"><a href="admin_main.jsp" class="nav-link text-white fs-4 active">어드민</a></li>
			<li class="nav-item">
				<a class="nav-link text-white" href="join_admin.jsp">등록</a>
			</li>
	      <li class="nav-item"><a href="trainer_main.jsp" class="nav-link text-gray">관제자</a></li>
	      <li class="nav-item"><a href="user_main.jsp" class="nav-link text-gray">훈련자</a></li>
      <hr>
    </ul>
  </div>
  <!-- /사이드바 -->

	<section>
		<div class="container">
			<div class="row mb-5">
				<div class="col-md-8 col-xl-9 text-center mx-auto">
                    <h1>어드민 목록</h1>
                </div>
            </div>
			<table class="table table-striped table-bordered table-dark table-hover" style="table-layout:fixed; border:1px solid #dddddd">
				<thead>
					<tr>
						<th class="bg-white text-black">UID</th>
						<th class="bg-white text-black">계정명</th>
						<th class="bg-white text-black">XR번호</th>
						<th class="bg-white text-black">이름</th>
					</tr>
				</thead>
				<tbody>
						<%
						DBWork db = new DBWork();
															ArrayList<User>	list = db.getAdminListLimit( mp.record_start_no );
															int i;
															for( i = 0; i < list.size(); ++i )
															{
						%>
					<tr onClick="location.href='view_admin.jsp?uid=<%=list.get(i).getUid()%>'">
						<td class="text-white"><%= list.get(i).getUid() %> </td>
						<td class="text-white"><%= list.get(i).getUserid() %></td>
						<td class="text-white"><%= list.get(i).getUsernumber() %></td>
						<td class="text-white"><%= list.get(i).getUsername() %></td>
					</tr>
						<%
							}
						%>
				</tbody>
			</table>
		</div>
    </section>
  
  
</div>		

<nav aria-label="Page navigation example">
	<ul class="pagination justify-content-center border-0">
		
		<%
		if( mp.pageno == 1 )
		{
		%>
			<li class="page-item disabled"><a class="page-link" href="admin_main.jsp?pageno=1">맨앞으로</a></li>
			<li class="page-item disabled"><a class="page-link" href="admin_main.jsp?pageno=<%=mp.prev_pageno%>">이전</a></li>
		<%
		}
		else
		{
		%>
			<li class="page-item"><a class="page-link text-black" href="admin_main.jsp?pageno=1">맨앞으로</a></li>
			<li class="page-item"><a class="page-link text-black" href="admin_main.jsp?pageno=<%=mp.prev_pageno%>">이전</a></li>
		<%
		}
		%>
		
		 
		<% 
		for(int ii = mp.page_sno; ii <= mp.page_eno; ii++ )
		{ 
		%>
				<%
				if( mp.pageno == ii)
				{ 
				%>
					<li class="page-item"><a class="page-link bg-black text-white" aria-current="page" href="admin_main.jsp?pageno=<%=ii %>"> <%=ii %></a></li>
				<%
				}
				else
				{ 
				%>
					<li class="page-item "><a class="page-link bg-white text-black" href="admin_main.jsp?pageno=<%=ii %>"> <%=ii %></a></li>
				<%
				} 
				%>
				 
		
		<%
		} 
		%>
		 	
		<%
		if(pageno == mp.total_page)
		{
		%>
		
			<li class="page-item disabled"><a class="page-link"  href="admin_main.jsp?pageno=<%=mp.next_pageno%>" >다음</a></li>
			<li class="page-item disabled"><a class="page-link" href="admin_main.jsp?pageno=<%=mp.total_page %>">맨뒤로</a></li>
		<%
		}
		else
		{
		%>
		
			<li class="page-item "><a class="page-link text-black"  href="admin_main.jsp?pageno=<%=mp.next_pageno%>" >다음</a></li>
			<li class="page-item "><a class="page-link text-black" href="admin_main.jsp?pageno=<%=mp.total_page %>">맨뒤로</a></li>
		<%
		}
		%>
		
		
	</ul>
</nav>

<footer class="mt-auto text-white-50 fixed-bottom">
	<p>Copyright ©2023 by <a href="https://interactcorp.co.kr" class="text-white">INTERACT Co.</a>, Ltd. All rights reserved.</p>
</footer>

</body>


</html>
