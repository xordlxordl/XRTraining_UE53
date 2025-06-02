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
	{//���� ������
		pageno = 1;
	}
	int total_record = 754;		   //�� ���ڵ� ��
	int page_per_record_cnt = 10;  //������ �� ���ڵ� ��
	int group_per_page_cnt =5;     //������ �� ������ ��ȣ ��[1],[2],[3],[4],[5]
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

	

// 	���� ������(����) / �������� �� ������ ���� ��ȣ ��(����) + (�׷� ��ȣ�� ���� ������(����) % �������� �� ������ ���� ��ȣ ��(����)>0 ? 1 : 0)
	int group_no = pageno / group_per_page_cnt + ( pageno % group_per_page_cnt > 0 ? 1:0 );
//		���� �׷��ȣ = ���������� / �������� ������ ��ȣ�� (���� ������ % �������� ������ ��ȣ �� >0 ? 1:0)	
//	ex) 	14		=	13(��)		=	 (66 / 5)		1	(1(������) =66 % 5)			  
	
	int page_eno = group_no * group_per_page_cnt;		
//		���� �׷� �� ��ȣ = ���� �׷��ȣ * �������� ������ ��ȣ 
//	ex) 	70		=	14	*	5
	int page_sno = page_eno - ( group_per_page_cnt - 1 );	
// 		���� �׷� ���� ��ȣ = ���� �׷� �� ��ȣ - (�������� ������ ��ȣ �� -1)
//	ex) 	66	=	70 - 	4 (5 -1)
	
	if( page_eno > total_page )
	{
//	   ���� �׷� �� ��ȣ�� ��ü������ �� ���� Ŭ ���		
		page_eno=total_page;
//	   ���� �׷� �� ��ȣ�� = ��ü������ ���� ����
	}
	
	int prev_pageno = page_sno - group_per_page_cnt;  // <<  *[����]* [21],[22],[23]... [30] [����]  >>
//		���� ������ ��ȣ	= ���� �׷� ���� ��ȣ - �������� ������ ��ȣ��	
//	ex)		46		=	51 - 5				
	int next_pageno = page_sno + group_per_page_cnt;	// <<  [����] [21],[22],[23]... [30] *[����]*  >>
//		���� ������ ��ȣ = ���� �׷� ���� ��ȣ + �������� ������ ��ȣ��
//	ex)		56		=	51 - 5
	if( prev_pageno < 1 )
	{
//		���� ������ ��ȣ�� 1���� ���� ���		
		prev_pageno=1;
//		���� �������� 1��
	}
	if( next_pageno > total_page )
	{
//		���� ���������� ��ü������ ������ Ŭ���		
		next_pageno = total_page / group_per_page_cnt * group_per_page_cnt + 1;
//		next_pageno=total_page
//		���� ������ = ��ü�������� / �������� ������ ��ȣ�� * �������� ������ ��ȣ�� + 1 
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

���� ������   (pageno)   : <%=pageno%><br />
��ü ������ ��   (total_record) : <%=total_record %><br />
�������� �� ���ڵ� ��   (page_per_record_cnt) : <%=page_per_record_cnt %><br />
�������� �� ������ ���� ��ȣ ��   (group_per_page_cnt) : <%=group_per_page_cnt %><br />

<hr />
���ڵ� ���� ��ȣ  (record_start_no) : <%=record_start_no%><br />
���ڵ� �� ��ȣ    (record_end_no) : <%=record_end_no %><br />
��ü������ ��     (total_page)  : <%=total_page %><br />
<hr />
���� �׷��ȣ [1] (group_no):  <%=group_no %><br />
���� �׷� ���� ��ȣ(page_sno): <%= page_sno%><br />
���� �׷� �� ��ȣ  (page_eno): <%= page_eno%><br />
���� ������ ��ȣ   (prev_pageno) <%=prev_pageno%><br />
���� ������ ��ȣ   (next_pageno) <%=next_pageno%><br />
<hr />


<a href="bbs.jsp?pageno=1">[�Ǿ�����]</a>
<a href="bbs.jsp?pageno=<%=prev_pageno%>">[����]</a> 
<% for(int i =page_sno;i<=page_eno;i++){ %>
	<a href="bbs.jsp?pageno=<%=i %>">
		<%if(pageno == i){ %>
			[<%=i %>]
		<%}else{ %>
			<%=i %>
		<%} %>
	</a> 
<%--	�޸�	 --%>	
	<%if( i < page_eno ){ %>
		,
	<%} %>
<%} %>

<a href="bbs.jsp?pageno=<%=next_pageno%>" >[����]</a>
<a href="bbs.jsp?pageno=<%=total_page %>">[�ǵڷ�]</a>


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
			<li class="page-item disabled"><a class="page-link" href="bbs.jsp?pageno=1">�Ǿ�����</a></li>
			<li class="page-item disabled"><a class="page-link" href="bbs.jsp?pageno=<%=prev_pageno%>">����</a></li>
		<%
		}
		else
		{
		%>
			<li class="page-item"><a class="page-link text-black" href="bbs.jsp?pageno=1">�Ǿ�����</a></li>
			<li class="page-item"><a class="page-link text-black" href="bbs.jsp?pageno=<%=prev_pageno%>">����</a></li>
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
		
			<li class="page-item disabled"><a class="page-link"  href="bbs.jsp?pageno=<%=next_pageno%>" >����</a></li>
			<li class="page-item disabled"><a class="page-link" href="bbs.jsp?pageno=<%=total_page %>">�ǵڷ�</a></li>
		<%
		}
		else
		{
		%>
		
			<li class="page-item "><a class="page-link text-black"  href="bbs.jsp?pageno=<%=next_pageno%>" >����</a></li>
			<li class="page-item "><a class="page-link text-black" href="bbs.jsp?pageno=<%=total_page %>">�ǵڷ�</a></li>
		<%
		}
		%>
		
		
	</ul>
</nav>
</body>

