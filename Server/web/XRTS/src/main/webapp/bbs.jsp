<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%!
	public Integer toInt( String x )
	{
		int a = 0;
		try
		{
			a = Integer.parseInt( x );
		}
		catch( Exception e )
		{
			
		}
		return a;
	}
%>
<%
	int pageno = toInt( request.getParameter("pageno") );
	if( pageno < 1 )
	{//현재 페이지
		pageno = 1;
	}
	int total_record = 754;		   //총 레코드 수
	int page_per_record_cnt = 10;  //페이지 당 레코드 수
	int group_per_page_cnt =5;     //페이지 당 보여줄 번호 수[1],[2],[3],[4],[5]
//					  									  [6],[7],[8],[9],[10]											

	int record_end_no = pageno * page_per_record_cnt;				
	int record_start_no = record_end_no - ( page_per_record_cnt - 1 );
	if( record_end_no > total_record )
	{
		record_end_no = total_record;
	}
										   
										   
	int total_page = total_record / page_per_record_cnt + ( total_record % page_per_record_cnt >0 ? 1 : 0 );
	if( pageno > total_page )
	{
		pageno = total_page;
	}

	

// 	현재 페이지(정수) / 한페이지 당 보여줄 페지 번호 수(정수) + (그룹 번호는 현제 페이지(정수) % 한페이지 당 보여줄 페지 번호 수(정수)>0 ? 1 : 0)
	int group_no = pageno / group_per_page_cnt + ( pageno % group_per_page_cnt > 0 ? 1:0 );
//		현재 그룹번호 = 현재페이지 / 페이지당 보여줄 번호수 (현재 페이지 % 페이지당 보여줄 번호 수 >0 ? 1:0)	
//	ex) 	14		=	13(몫)		=	 (66 / 5)		1	(1(나머지) =66 % 5)			  
	
	int page_eno = group_no * group_per_page_cnt;		
//		현재 그룹 끝 번호 = 현재 그룹번호 * 페이지당 보여줄 번호 
//	ex) 	70		=	14	*	5
	int page_sno = page_eno - ( group_per_page_cnt - 1 );	
// 		현재 그룹 시작 번호 = 현재 그룹 끝 번호 - (페이지당 보여줄 번호 수 -1)
//	ex) 	66	=	70 - 	4 (5 -1)
	
	if( page_eno > total_page )
	{
//	   현재 그룹 끝 번호가 전체페이지 수 보다 클 경우		
		page_eno=total_page;
//	   현재 그룹 끝 번호와 = 전체페이지 수를 같게
	}
	
	int prev_pageno = page_sno - group_per_page_cnt;  // <<  *[이전]* [21],[22],[23]... [30] [다음]  >>
//		이전 페이지 번호	= 현재 그룹 시작 번호 - 페이지당 보여줄 번호수	
//	ex)		46		=	51 - 5				
	int next_pageno = page_sno + group_per_page_cnt;	// <<  [이전] [21],[22],[23]... [30] *[다음]*  >>
//		다음 페이지 번호 = 현재 그룹 시작 번호 + 페이지당 보여줄 번호수
//	ex)		56		=	51 - 5
	if( prev_pageno < 1 )
	{
//		이전 페이지 번호가 1보다 작을 경우		
		prev_pageno=1;
//		이전 페이지를 1로
	}
	if( next_pageno > total_page )
	{
//		다음 페이지보다 전체페이지 수보가 클경우		
		next_pageno = total_page / group_per_page_cnt * group_per_page_cnt + 1;
//		next_pageno=total_page
//		다음 페이지 = 전체페이지수 / 페이지당 보여줄 번호수 * 페이지당 보여줄 번호수 + 1 
//	ex)			   = 	76 / 5 * 5 + 1	???????? 		
	}
	
	// [1][2][3].[10]
	// [11][12]
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
전체 데이터 수   (total_record) : <%=total_record %><br />
한페이지 당 레코드 수   (page_per_record_cnt) : <%=page_per_record_cnt %><br />
한페이지 당 보여줄 페지 번호 수   (group_per_page_cnt) : <%=group_per_page_cnt %><br />

<hr />
레코드 시작 번호  (record_start_no) : <%=record_start_no%><br />
레코드 끝 번호    (record_end_no) : <%=record_end_no %><br />
전체페이지 수     (total_page)  : <%=total_page %><br />
<hr />
현재 그룹번호 [1] (group_no):  <%=group_no %><br />
현재 그룹 시작 번호(page_sno): <%= page_sno%><br />
현재 그룹 끝 번호  (page_eno): <%= page_eno%><br />
이전 페이지 번호   (prev_pageno) <%=prev_pageno%><br />
다음 페이지 번호   (next_pageno) <%=next_pageno%><br />
<hr />


<a href="bbs.jsp?pageno=1">[맨앞으로]</a>
<a href="bbs.jsp?pageno=<%=prev_pageno%>">[이전]</a> 
<% for(int i =page_sno;i<=page_eno;i++){ %>
	<a href="bbs.jsp?pageno=<%=i %>">
		<%if(pageno == i){ %>
			[<%=i %>]
		<%}else{ %>
			<%=i %>
		<%} %>
	</a> 
<%--	콤마	 --%>	
	<%if( i < page_eno ){ %>
		,
	<%} %>
<%} %>

<a href="bbs.jsp?pageno=<%=next_pageno%>" >[다음]</a>
<a href="bbs.jsp?pageno=<%=total_page %>">[맨뒤로]</a>


<br>

<br>
<hr/>
<br>

<nav aria-label="Page navigation example">
	<ul class="pagination justify-content-center border-0">
		
		<%
		if(pageno == 1)
		{
		%>
			<li class="page-item disabled"><a class="page-link" href="bbs.jsp?pageno=1">맨앞으로</a></li>
			<li class="page-item disabled"><a class="page-link" href="bbs.jsp?pageno=<%=prev_pageno%>">이전</a></li>
		<%
		}
		else
		{
		%>
			<li class="page-item"><a class="page-link text-black" href="bbs.jsp?pageno=1">맨앞으로</a></li>
			<li class="page-item"><a class="page-link text-black" href="bbs.jsp?pageno=<%=prev_pageno%>">이전</a></li>
		<%
		}
		%>
		
		 
		<% 
		for(int i =page_sno;i<=page_eno;i++)
		{ 
		%>
				<%
				if(pageno == i)
				{ 
				%>
					<li class="page-item"><a class="page-link bg-black text-white" aria-current="page" href="bbs.jsp?pageno=<%=i %>"> <%=i %>
				<%
				}
				else
				{ 
				%>
					<li class="page-item "><a class="page-link bg-white text-black" href="bbs.jsp?pageno=<%=i %>"> <%=i %>
				<%
				} 
				%>
			</a></li> 
		
		<%
		} 
		%>
		 	
		<%
		if(pageno == total_page)
		{
		%>
		
			<li class="page-item disabled"><a class="page-link"  href="bbs.jsp?pageno=<%=next_pageno%>" >다음</a></li>
			<li class="page-item disabled"><a class="page-link" href="bbs.jsp?pageno=<%=total_page %>">맨뒤로</a></li>
		<%
		}
		else
		{
		%>
		
			<li class="page-item "><a class="page-link text-black"  href="bbs.jsp?pageno=<%=next_pageno%>" >다음</a></li>
			<li class="page-item "><a class="page-link text-black" href="bbs.jsp?pageno=<%=total_page %>">맨뒤로</a></li>
		<%
		}
		%>
		
		
	</ul>
</nav>
</body>

