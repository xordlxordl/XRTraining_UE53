
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

function detectEncoding(text) {
    // UTF-8 BOM (Byte Order Mark): EF BB BF
    if (text.charCodeAt(0) === 0xFEFF) {
        return "UTF-8";
    }

    // Check for UTF-16 LE BOM: FF FE
    if (text.charCodeAt(0) === 0xFF && text.charCodeAt(1) === 0xFE) {
        return "UTF-16LE";
    }

    // Check for UTF-16 BE BOM: FE FF
    if (text.charCodeAt(0) === 0xFE && text.charCodeAt(1) === 0xFF) {
        return "UTF-16BE";
    }

    // Simple heuristic to guess ANSI if no BOM found
    // Check if there are any non-ASCII characters
    for (let i = 0; i < text.length; i++) {
        if (text.charCodeAt(i) > 127) {
            return "ANSI";
        }
    }

    // Default to UTF-8 if no BOM and no non-ASCII characters
    return "UTF-8";
} 

function tabulate(data) 
{
	
	$('#id_mainhead').remove();
	$('#id_maintbody').remove();
	
	const body = d3.select('tbody');
	body.remove();
	
	$("#id_maintable").html("")

	
	//
	const table = d3.select('#id_maintable');
	
	
	const thead = table.append('thead')
	const tbody = table.append('tbody')

	d3.select('thead')
	.attr('id','id_mainhead')
    
    d3.select('tbody')
	.attr('id','id_maintbody')

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
		//let encode = detectEncoding( dataUrl);
		
		//alert(encode);
		
		previewCSVData(dataUrl);

            //document.getElementById('out').textContent = reader.result;
	}
	//reader.readAsDataURL(file,"euc-kr");
	reader.readAsText( file, "euc-kr");
});
    
let table;
const previewCSVData = async dataurl => 
{
	//datatable 초기화
	//let table = $('#id_maintable').DataTable 생성자 호출 전에 호출 해야함
	if ( $.fn.DataTable.isDataTable( '#id_maintable' ) ) 
	{
		$('#id_maintable').DataTable().destroy();
  		// dynamically하게 column이 들어가야하므로
		$('#id_maintable').empty(); 	
	};
	
	d3.text(dataurl)
	.then( d3.csvParseRows )
	.then( tabulate )
	.then(() => $(document).ready( function ()
	{  

		
		table = $('#id_maintable').DataTable(
		{
			"responsive": true,
			"autoWidth": true,
			"destroy": true,
			"remove":true,
			"ordering":false, 
			"serverSide": false,
			"retrieve": true,
			"language":lang_kor,
			"jQueryUI": true,
			//"api":true,
			"visible":true,
			"pagingType": 'full_numbers_no_ellipses',
		    "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
			"pageLength": 10,
		})
		
		var status = $("#id_maintbody").css("display");
		if( status == "none" ) 
		{
			$("#id_maintbody").css("display", "");
		}
		
		var btn = $("#id_savebtn").css("display");
		if( btn == "none" ) 
		{
			$("#id_savebtn").css("display", "");
		}
		
	}
  
  ))
}

function ErrorMsg(msg)
{
	Swal.fire({
  title: msg,
  icon: 'error',
  showConfirmButton: true,
  confirmButtonColor: '#d33',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
   
  }
})
}

function InsertUser()
{
     var datas = table.rows().data().toArray();
     
     
     

     var rowcount = datas.length;
     
     
     if( rowcount <= 0 )
     {
		 ErrorMsg('데이터가 없습니다.' );
		 return;
	 }
     
     var colcount = datas[0].length;
     if( colcount != 3 )
     {
		 ErrorMsg('포멧에 맞지 않는 문서입니다.');
		 return;
	 }
     
     for( var i = 0; i < rowcount; i++ )
     {
		 
		 
		var str1 = datas[i][0];
		var str2 = datas[i][1];
		var str3 = datas[i][2];
		
		if( str2.length < 8 )
		{
			
			ErrorMsg( str1 + ' ' + str2 + ' 군번 오류' );
			return;
		}
	 }

	
	 document.getElementById("id_query").value = datas;
	
	let myform = document.querySelector("form");
	if( myform.requestSubmit ) 
	{
		myform.requestSubmit();
	} 
	else 
	{
	
		myform.submit();
	}
	
}

