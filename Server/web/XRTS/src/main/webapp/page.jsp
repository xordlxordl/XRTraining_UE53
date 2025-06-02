<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="data.*" %>

<%@ page import="db.*" %>


<%
	
	MakePage mp = new MakePage();
	int pageno = mp.toInt( request.getParameter("pageno") );
	mp.total_record = 9999;
	mp.Make( pageno );

%>


<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/docs.css">
	<link rel="stylesheet" href="css/style.css">
	<link rel="stylesheet" href="css/sweetalert2.min.css">
	
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/bootstrap.bundle.min.js"></script>
	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/sweetalert2.all.min.js"></script>
	<script src="js/my.js"></script>
	
	<title>XR TRANING SYTEM</title>
</head>	
<body class="h-75 text-center text-bg-dark">

현재 페이지   (pageno)   : <%=pageno%><br />
전체 데이터 수   (total_record) : <%=mp.total_record %><br />
한페이지 당 레코드 수   (page_per_record_cnt) : <%=mp.page_per_record_cnt %><br />
한페이지 당 보여줄 페지 번호 수   (group_per_page_cnt) : <%=mp.group_per_page_cnt %><br />

<hr />
레코드 시작 번호  (record_start_no) : <%=mp.record_start_no%><br />
레코드 끝 번호    (record_end_no) : <%=mp.record_end_no %><br />
전체페이지 수     (total_page)  : <%=mp.total_page %><br />
<hr />
현재 그룹번호 [1] (group_no):  <%=mp.group_no %><br />
현재 그룹 시작 번호(page_sno): <%= mp.page_sno%><br />
현재 그룹 끝 번호  (page_eno): <%= mp.page_eno%><br />
이전 페이지 번호   (prev_pageno) <%=mp.prev_pageno%><br />
다음 페이지 번호   (next_pageno) <%=mp.next_pageno%><br />
<hr />


<a href="bbs.jsp?pageno=1">[맨앞으로]</a>
<a href="bbs.jsp?pageno=<%=mp.prev_pageno%>">[이전]</a> 
<% for(int i =mp.page_sno;i<=mp.page_eno;i++){ %>
	<a href="bbs.jsp?pageno=<%=i %>">
		<%if(pageno == i){ %>
			[<%=i %>]
		<%}else{ %>
			<%=i %>
		<%} %>
	</a> 
<%--	콤마	 --%>	
	<%if( i < mp.page_eno ){ %>
		,
	<%} %>
<%} %>

<a href="bbs.jsp?pageno=<%=mp.next_pageno%>" >[다음]</a>
<a href="bbs.jsp?pageno=<%=mp.total_page %>">[맨뒤로]</a>


<br>

<br>
<hr/>
<br>

<nav aria-label="Page navigation example">
	<ul class="pagination justify-content-center border-0">
		
		<%
		if( mp.pageno == 1 )
		{
		%>
			<li class="page-item disabled"><a class="page-link" href="page.jsp?pageno=1">맨앞으로</a></li>
			<li class="page-item disabled"><a class="page-link" href="page.jsp?pageno=<%=mp.prev_pageno%>">이전</a></li>
		<%
		}
		else
		{
		%>
			<li class="page-item"><a class="page-link text-black" href="page.jsp?pageno=1">맨앞으로</a></li>
			<li class="page-item"><a class="page-link text-black" href="page.jsp?pageno=<%=mp.prev_pageno%>">이전</a></li>
		<%
		}
		%>
		
		 
		<% 
		for(int i =mp.page_sno;i<=mp.page_eno;i++)
		{ 
		%>
				<%
				if( mp.pageno == i)
				{ 
				%>
					<li class="page-item"><a class="page-link bg-black text-white" aria-current="page" href="page.jsp?pageno=<%=i %>"> <%=i %>
				<%
				}
				else
				{ 
				%>
					<li class="page-item "><a class="page-link bg-white text-black" href="page.jsp?pageno=<%=i %>"> <%=i %>
				<%
				} 
				%>
			</a></li> 
		
		<%
		} 
		%>
		 	
		<%
		if(pageno == mp.total_page)
		{
		%>
		
			<li class="page-item disabled"><a class="page-link"  href="page.jsp?pageno=<%=mp.next_pageno%>" >다음</a></li>
			<li class="page-item disabled"><a class="page-link" href="page.jsp?pageno=<%=mp.total_page %>">맨뒤로</a></li>
		<%
		}
		else
		{
		%>
		
			<li class="page-item "><a class="page-link text-black"  href="page.jsp?pageno=<%=mp.next_pageno%>" >다음</a></li>
			<li class="page-item "><a class="page-link text-black" href="page.jsp?pageno=<%=mp.total_page %>">맨뒤로</a></li>
		<%
		}
		%>
		
		
	</ul>
</nav>
</body>

