/**
 * 
 */




var this_row;

 var lang_kor = {
        "decimal" : "",
        "emptyTable" : "데이터가 없습니다.",
        "info" : "_START_ - _END_ (총 _TOTAL_ 개)",
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
    var table = $('#id_maintable1').DataTable({
		
		"responsive": false,
		"autoWidth": true,
		"destroy": true,
		"serverSide": false,
		"language":lang_kor,
        "searching":false,
        "lengthChange":false,
		"pageLength": 10,
		"info":false,
		"paging":false
	})
 
 	var status = $("#id_maintbody1").css("display");
	if( status == "none" ) 
    {
		$("#id_maintbody1").css("display", "");
	}
});




function GoToTRUserList(i)
{
    location.href = 'tr_user_list.jsp?tid='+ i;
}


