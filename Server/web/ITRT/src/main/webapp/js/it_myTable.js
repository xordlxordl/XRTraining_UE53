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
            if (status == "none") 
            {
                $("#id_maintbody").css("display", "");
            }
        
    $('#id_maintbody').on('click', 'tr', function () 
    {

        if ($(this).hasClass('selected')) 
        {
            $(this).removeClass('selected');
			
			var con = document.getElementById("id_modyfymenu");
			con.style.display = 'none';
			
			var condel = document.getElementById("id_delmenu");
			condel.style.display = 'none';
			
			var conprod = document.getElementById("id_prodmenu");
			conprod.style.display = 'none';
			
			var conaddprod = document.getElementById("id_addprodmenu");
			conaddprod.style.display = 'none';
			
        } 
        else 
        {
            table.$('tr.selected').removeClass('selected');
            $(this).addClass('selected');
            
            var con = document.getElementById("id_modyfymenu");
			con.style.display = 'block';
            
            
            var condel = document.getElementById("id_delmenu");
			condel.style.display = 'block';
            
            var conprod = document.getElementById("id_prodmenu");
			conprod.style.display = 'block';
         
         	var conaddprod = document.getElementById("id_addprodmenu");
			conaddprod.style.display = 'block';
            
            this_row = table.rows(this).data();
        }
    });


});


function DelUser()
{
	Swal.fire({
  title: '['+ this_row[0][1]+']계정' + ' 데이터를 삭제합니다',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'it_DeleteUserAction.jsp?uid='+this_row[0][0] +'&username='+ this_row[0][1];
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}


function GoToUserEdit()
{
	//alert('uid='+ this_row[0][0]);
    location.href = 'it_edit_user.jsp?uid='+ this_row[0][0];
}

function GetUserProd()
{
    location.href = 'it_view_prod.jsp?uid='+ this_row[0][0];
}

function AddUserProd()
{
    location.href = 'it_add_userprod2.jsp?uid='+ this_row[0][0];
}



