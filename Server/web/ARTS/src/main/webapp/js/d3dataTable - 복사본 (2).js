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

 function Geeks()
 { 
	 
	 
	const table = document.getElementById('id_maintable');
	const totalRowCnt = table.rows.length;
 
 	for( var i = 0; i < totalRowCnt; ++i)
 	{
		table.rows[i].deleteCell(-1);	 
	}
	
 /*
	var table = document.getElementById('id_maintable'); 
	var row = table.rows; // Getting the rows 
  	var cells = row[0].cells;
    for (var i = 0; i < cells.length ; i++) 
    { 
    	for (var j = 0; j < row.length ; j++) 
        { 
                   
        	row[j].deleteCell(-1);
		}
		//cells[i].deleteCell(-1);
	} 
 */
} 

function tabulate(data) 
{
	$("#id_maintable").html("");
	
	d3.select('thead').remove();
	d3.select('tbody').remove();
	
	const table = d3.select('#id_maintable');
	
	
	//document.getElementById('id_maintable').remove();
	
	const thead = table.append('thead')
	const tbody = table.append('tbody')

  thead.append('tr')
    .selectAll(null)
    .data(data.shift())
    .enter()
    .append('th')
    .attr('class','bg-white text-black')
    .text(d => d)

	d3.select('tbody')
	.attr('id','id_maintbody')
    .style('display','none')

  const rows = tbody.selectAll(null)
    .data(data)
    .enter()
    .append('tr')

  rows.selectAll()
    .data(d => d)
    .enter()
    .append('td')
    .text(d => d)

  return table;
}

$(document).on("change", "#csv", function()
{
	const fileInput = document.getElementById('csv')
	const file = fileInput.files[0]
	const reader = new FileReader()
	reader.onload = () =>
	{

		const dataUrl = reader.result;
		previewCSVData(dataUrl);

            //document.getElementById('out').textContent = reader.result;
	}
	reader.readAsDataURL(file)
});

const previewCSVData = async dataurl => 
{

    d3.text(dataurl)
    .then(d3.csvParseRows)
    .then(tabulate)
    .then(() => 
    {
        if ($.fn.DataTable.isDataTable('#id_maintable')) 
        {
            $('#id_maintable').DataTable().destroy(); // 기존 DataTable 인스턴스 파괴
        }

        let table = $('#id_maintable').DataTable({
            "responsive": true,
            "autoWidth": true,
            "destroy": true,
            "remove": true,
            "ordering": false,
            "serverSide": false,
            "retrieve": true,
            "language": lang_kor,
            "jQueryUI": true,
            "pagingType": 'full_numbers_no_ellipses',
            "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20", "50", "100", "전체"]],
            "pageLength": 10,
        });

        var status = $("#id_maintbody").css("display");
        if( status == "none" ) 
        {
            $("#id_maintbody").css("display", "");
        }
    });
}
