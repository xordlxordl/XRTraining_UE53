package db;

public class MakePage
{
	
	public int pageno;
	public int total_record = 0;		   //총 레코드 수
	public int page_per_record_cnt = 10;  //페이지 당 레코드 수
	public int group_per_page_cnt = 10;     //페이지 당 보여줄 번호 수[1],[2],[3],[4],[5]
	public int record_end_no;
	public int record_start_no;
	public int total_page;
	public int group_no;
	public int page_eno;
	public int page_sno;
	public int prev_pageno;
	public int next_pageno;
	
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
	
	public void Make( int page )
	{
		pageno = page;
		if( pageno < 1 )
		{//현재 페이지
			pageno = 1;
		}
		
//						  									  [6],[7],[8],[9],[10]											

		record_end_no = pageno * page_per_record_cnt;				
		record_start_no = record_end_no - ( page_per_record_cnt - 1 );
		if( record_end_no > total_record )
		{
			record_end_no = total_record;
		}
											   
											   
		total_page = total_record / page_per_record_cnt + ( total_record % page_per_record_cnt >0 ? 1 : 0 );
		if( pageno > total_page )
		{
			pageno = total_page;
		}

		

//	 	현재 페이지(정수) / 한페이지 당 보여줄 페지 번호 수(정수) + (그룹 번호는 현제 페이지(정수) % 한페이지 당 보여줄 페지 번호 수(정수)>0 ? 1 : 0)
		group_no = pageno / group_per_page_cnt + ( pageno % group_per_page_cnt > 0 ? 1:0 );
//			현재 그룹번호 = 현재페이지 / 페이지당 보여줄 번호수 (현재 페이지 % 페이지당 보여줄 번호 수 >0 ? 1:0)	
//		ex) 	14		=	13(몫)		=	 (66 / 5)		1	(1(나머지) =66 % 5)			  
		
		page_eno = group_no * group_per_page_cnt;		
//			현재 그룹 끝 번호 = 현재 그룹번호 * 페이지당 보여줄 번호 
//		ex) 	70		=	14	*	5
		page_sno = page_eno - ( group_per_page_cnt - 1 );	
//	 		현재 그룹 시작 번호 = 현재 그룹 끝 번호 - (페이지당 보여줄 번호 수 -1)
//		ex) 	66	=	70 - 	4 (5 -1)
		
		if( page_eno > total_page )
		{
//		   현재 그룹 끝 번호가 전체페이지 수 보다 클 경우		
			page_eno=total_page;
//		   현재 그룹 끝 번호와 = 전체페이지 수를 같게
		}
		
		prev_pageno = page_sno - group_per_page_cnt;  // <<  *[이전]* [21],[22],[23]... [30] [다음]  >>
//			이전 페이지 번호	= 현재 그룹 시작 번호 - 페이지당 보여줄 번호수	
//		ex)		46		=	51 - 5				
		next_pageno = page_sno + group_per_page_cnt;	// <<  [이전] [21],[22],[23]... [30] *[다음]*  >>
//			다음 페이지 번호 = 현재 그룹 시작 번호 + 페이지당 보여줄 번호수
//		ex)		56		=	51 - 5
		if( prev_pageno < 1 )
		{
//			이전 페이지 번호가 1보다 작을 경우		
			prev_pageno=1;
//			이전 페이지를 1로
		}
		if( next_pageno > total_page )
		{
//			다음 페이지보다 전체페이지 수보가 클경우		
			next_pageno = total_page / group_per_page_cnt * group_per_page_cnt + 1;
//			next_pageno=total_page
//			다음 페이지 = 전체페이지수 / 페이지당 보여줄 번호수 * 페이지당 보여줄 번호수 + 1 
//		ex)			   = 	76 / 5 * 5 + 1	???????? 		
		}
		
	}
}
