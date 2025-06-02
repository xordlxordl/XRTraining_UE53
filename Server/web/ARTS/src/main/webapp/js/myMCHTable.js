/**
 * 
 */




var this_row;

 var lang_kor = {
        "decimal" : "",
        "emptyTable" : "파일이 없습니다.",
        "info" : "_START_ - _END_ (총 _TOTAL_ 개 파일)",
        "infoEmpty" : "0개 파일",
        "infoFiltered" : "(전체 _MAX_ 개 중 검색결과)",
        "infoPostFix" : "",
        "thousands" : ",",
        "lengthMenu" : "_MENU_ 개씩 보기",
        "loadingRecords" : "로딩중...",
        "processing" : "처리중...",
        "search" : "검색 : ",
        "zeroRecords" : "검색 된 파일이 없습니다.",
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
    $('#id_maintable').DataTable({
	
    
		"responsive": true,
		"autoWidth": false,
		"destroy": true,
		"serverSide": false,
		"language":lang_kor,
		"jQueryUI": true,
		"select":true,
		"footer":true,
		"pagingType": 'full_numbers_no_ellipses',
        "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
		"pageLength": 10,
		
	});
 
 	var status = $("#id_maintbody").css("display");
    if (status == "none") 
    {
        $("#id_maintbody").css("display", "");
    }
 

});

function GoToMCHEditWithIndex(i)
{
   	location.href = 'ar_mch_edit.jsp?uid='+ i;
}
