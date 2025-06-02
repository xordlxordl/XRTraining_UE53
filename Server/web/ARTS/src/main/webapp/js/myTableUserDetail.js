/**
 * 
 */



var this_row;

 var lang_kor = {
        "decimal" : "",
        "emptyTable" : "데이터가 없습니다.",
        "info" : "_START_ - _END_ (총 _TOTAL_ 명)",
        "infoEmpty" : "0명",
        "infoFiltered" : "(전체 _MAX_ 명 중 검색결과)",
        "infoPostFix" : "",
        "thousands" : ",",
        "lengthMenu" : "_MENU_ 개씩 보기",
        "loadingRecords" : "로딩중...",
        "processing" : "처리중...",
        "search" : "검색 : ",
        "zeroRecords" : "검색된 데이터가 없습니다.",
        "paginate" : {
            "first" : "첫 페이지",
            "last" : "마지막 페이지",
            "next" : "다음",
            "previous" : "이전"
        },
        "aria" : {
            "sortAscending" : " :  오름차순 정렬",
            "sortDescending" : " :  내림차순 정렬"
        }
    };
    
$(document).ready( function () 
{
    var table = $('#id_maintable').DataTable({
		
		"responsive": true,
		"autoWidth": true,
		"destroy": true,
		"serverSide": false,
		"language":lang_kor,
		"jQueryUI": true,
		"searching":false,
		"ordering": false,
		"info":false,
		"pagingType": 'simple',
        "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
        "paging": false,
		"pageLength": 1,
		"lengthChange":true,
		
		 "layout":
		 {
			 topEnd:
			 {
				buttons:
				[{
					extend:'print',
					text:'&nbsp;&nbsp;<i class="fa fa-print" style="color:white">&nbsp;&nbsp;</i>프린트 하기&nbsp;&nbsp;',
					autoPrint:true,
					exportOptions:
					{
						 columns: ':visible',
						stripHtml: false,
						 
      				},
					customize: function ( win ) 
					{

            			$(win.document.body).find( 'table' )
                		.removeClass('table-dark')
                		.removeClass('table-hover')
                		.removeClass('table-striped')
                		.addClass('display')
 						.css('text-align','center');
	
        			}
        		 }]				 
			 }
		 }
	})
 
 	var status = $("#id_maintbody").css("display");
	if( status == "none" ) 
    {
		$("#id_maintbody").css("display", "");
	}
});
