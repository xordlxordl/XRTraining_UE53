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

���� ������   (pageno)   : <%=pageno%><br />
��ü ������ ��   (total_record) : <%=mp.total_record %><br />
�������� �� ���ڵ� ��   (page_per_record_cnt) : <%=mp.page_per_record_cnt %><br />
�������� �� ������ ���� ��ȣ ��   (group_per_page_cnt) : <%=mp.group_per_page_cnt %><br />

<hr />
���ڵ� ���� ��ȣ  (record_start_no) : <%=mp.record_start_no%><br />
���ڵ� �� ��ȣ    (record_end_no) : <%=mp.record_end_no %><br />
��ü������ ��     (total_page)  : <%=mp.total_page %><br />
<hr />
���� �׷��ȣ [1] (group_no):  <%=mp.group_no %><br />
���� �׷� ���� ��ȣ(page_sno): <%= mp.page_sno%><br />
���� �׷� �� ��ȣ  (page_eno): <%= mp.page_eno%><br />
���� ������ ��ȣ   (prev_pageno) <%=mp.prev_pageno%><br />
���� ������ ��ȣ   (next_pageno) <%=mp.next_pageno%><br />
<hr />


<a href="bbs.jsp?pageno=1">[�Ǿ�����]</a>
<a href="bbs.jsp?pageno=<%=mp.prev_pageno%>">[����]</a> 
<% for(int i =mp.page_sno;i<=mp.page_eno;i++){ %>
	<a href="bbs.jsp?pageno=<%=i %>">
		<%if(pageno == i){ %>
			[<%=i %>]
		<%}else{ %>
			<%=i %>
		<%} %>
	</a> 
<%--	�޸�	 --%>	
	<%if( i < mp.page_eno ){ %>
		,
	<%} %>
<%} %>

<a href="bbs.jsp?pageno=<%=mp.next_pageno%>" >[����]</a>
<a href="bbs.jsp?pageno=<%=mp.total_page %>">[�ǵڷ�]</a>


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
			<li class="page-item disabled"><a class="page-link" href="page.jsp?pageno=1">�Ǿ�����</a></li>
			<li class="page-item disabled"><a class="page-link" href="page.jsp?pageno=<%=mp.prev_pageno%>">����</a></li>
		<%
		}
		else
		{
		%>
			<li class="page-item"><a class="page-link text-black" href="page.jsp?pageno=1">�Ǿ�����</a></li>
			<li class="page-item"><a class="page-link text-black" href="page.jsp?pageno=<%=mp.prev_pageno%>">����</a></li>
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
		
			<li class="page-item disabled"><a class="page-link"  href="page.jsp?pageno=<%=mp.next_pageno%>" >����</a></li>
			<li class="page-item disabled"><a class="page-link" href="page.jsp?pageno=<%=mp.total_page %>">�ǵڷ�</a></li>
		<%
		}
		else
		{
		%>
		
			<li class="page-item "><a class="page-link text-black"  href="page.jsp?pageno=<%=mp.next_pageno%>" >����</a></li>
			<li class="page-item "><a class="page-link text-black" href="page.jsp?pageno=<%=mp.total_page %>">�ǵڷ�</a></li>
		<%
		}
		%>
		
		
	</ul>
</nav>
</body>

