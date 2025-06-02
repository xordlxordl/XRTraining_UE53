/**
 * 
 */




var this_row;

 var lang_kor = {
        "decimal" : "",
        "emptyTable" : "데이터가 없습니다.",
        "info" : "_START_ - _END_ (총 _TOTAL_ 개)",
        "infoEmpty" : "0개",
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
        } 
        else 
        {
            table.$('tr.selected').removeClass('selected');
            $(this).addClass('selected');
            
            var con = document.getElementById("id_modyfymenu");
            
			con.style.display = 'block';
            
            this_row = table.rows(this).data();
        }
    });


});

function GoToServerEdit()
{
    location.href = 'edit_server.jsp?&nid='+ this_row[0][0] +'&mapid='+ this_row[0][1] +'&name='+ this_row[0][2] +'&ip='+ this_row[0][3] +'&port='+ this_row[0][4];
}

function GoToXREdit()
{
    location.href = 'edit_xr.jsp?&nid='+ this_row[0][0] +'&name='+ this_row[0][1];
}

function GoToMapEdit()
{
    location.href = 'edit_map.jsp?&nid='+ this_row[0][0] +'&name='+ this_row[0][1];
}

function GoToTrainingEdit()
{
    location.href = 'edit_training.jsp?&nid='+ this_row[0][0] +'&name='+ this_row[0][1];
}