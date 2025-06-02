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
		"pagingType": 'full_numbers_no_ellipses',
        "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
		"pageLength": 10,
	})
 
 	var status = $("#id_maintbody").css("display");
	if( status == "none" ) 
    {
		$("#id_maintbody").css("display", "");
	}
});


function DelUser()
{
	Swal.fire({
  title: '['+ this_row[0][1]+']님' + ' 데이터를 삭제합니다',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'dr_DeleteUserAction.jsp?uid='+this_row[0][0] +'&username='+ this_row[0][1];
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function GoToUserEditWithIndex(i)
{
    location.href = 'ar_user_edit.jsp?uid='+ i;
}

function GoToTrUserLog(i)
{
	location.href = 'tr_user_log.jsp?uid='+ i;
}

function GoToTRUserList(i)
{
    location.href = 'tr_user_list.jsp?tid='+ i;
}

function GoToUserEdit()
{
    location.href = 'ar_user_edit.jsp?uid='+ this_row[0][0];
}

function GoToSimView()
{
    location.href = 'dr_view_sim.jsp?uid='+ this_row[0][0];
}
