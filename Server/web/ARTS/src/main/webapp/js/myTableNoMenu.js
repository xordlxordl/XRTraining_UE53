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
    
   
 /*  
$(document).ready( function () 
{
    var table = $('#id_maintable').DataTable({
		
		"responsive": true,
		"autoWidth": true,
		"destroy": true,
		"serverSide": false,
		"language":lang_kor,
		"jQueryUI": true,
		"destroy":true,
		"pagingType": 'full_numbers_no_ellipses',
        "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
		"pageLength": 10,
		 columns: [{
        "title": "ID",
        "data": "ID"
    }, {
        "title": "Name",
        "data": "Name"
    }]
	})

});
*/
/*
    var table = $('#id_maintable').DataTable({
		
		"responsive": true,
		"autoWidth": true,
		"destroy": true,
		"serverSide": false,
		"language":lang_kor,
		"jQueryUI": true,
		"destroy":true,
		"pagingType": 'full_numbers_no_ellipses',
        "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
		"pageLength": 10,
		 columns: [{
        "title": "ID",
        "data": "ID"
    }, {
        "title": "Name",
        "data": "Name"
    }]
	});
	*/


function loadcsv()
{
	 var table = $('#id_maintable').DataTable({
		
		"responsive": true,
		"autoWidth": true,
		"destroy": true,
		"serverSide": false,
		"language":lang_kor,
		"jQueryUI": true,
		"destroy":true,
		"pagingType": 'full_numbers_no_ellipses',
        "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
		"pageLength": 10,
		 columns: [{
        "title": "Type",
        "data": "Type"
    }, {
        "title": "Name",
        "data": "Name"
    }]
	})

	
	  
    var regex = /^([a-zA-Z0-9\s_\\.\-:])+(.csv|.txt)$/;
    
    if (regex.test($("#inputGroupFile02").val().toLowerCase())) 
    {
        if (typeof (FileReader) != "undefined") 
        {
            var reader = new FileReader();
            reader.onload = function (e) 
            {
                //var customers = new Array();
                var rows = e.target.result.split("\r\n");
                
                
                
    			console.log( rows );
                table.rows.add( rows ).draw();
            }
            reader.readAsText($("#inputGroupFile02")[0].files[0]);
        } 
    }
    
      
}

/*
 function loadcsv() 
  {
	  
	  var table = $('#id_maintable').DataTable(
		  {
			  "responsive": true,
		"autoWidth": true,
		"destroy": true,
		"serverSide": false,
		"language":lang_kor,
		"jQueryUI": true,
		"destroy":true,
		"pagingType": 'full_numbers_no_ellipses',
        "aLengthMenu": [[10, 20, 50, 100, -1], ["10", "20","50", "100", "전체"]],
		"pageLength": 10		
		  } );
	  
	  var fileUpload = document.getElementById("inputGroupFile02");
	  var regex = /^([a-zA-Z0-9\s_\\.\-:])+(.csv|.txt)$/;
	  
		if( regex.test( fileUpload.value.toLowerCase() ) ) 
		{
			if( typeof( FileReader ) != "undefined" ) 
            {
				var reader = new FileReader();
				
					 
				reader.readAsText(fileUpload.files[0], "UTF-8");
				reader.onload = function(event)
				{
					var csv = event.target.result;
			        var data = $.csv.toArrays(csv);
			        var html = '';
			        for(var row in data) 
			        {
			          html += '<tr>\r\n';
			          for(var item in data[row]) 
			          {
			            html += '<td>' + data[row][item] + '</td>\r\n';
			          }
			          html += '</tr>\r\n';
			        }
			         $('#id_maintable').html(html);
				} 
				
			}
		}
		
}
*/
/*
  function loadcsv() 
  {
	  
	  var table = $('#id_maintable').DataTable();
	  
	  var fileUpload = document.getElementById("inputGroupFile02");
	  var regex = /^([a-zA-Z0-9\s_\\.\-:])+(.csv|.txt)$/;
	  
		if( regex.test( fileUpload.value.toLowerCase() ) ) 
		{
			if( typeof( FileReader ) != "undefined" ) 
            {
				var reader = new FileReader();
				
				//reader.onload = function( e ) 
				reader.onloadend = function( e )
				{
					//var table = document.getElementById( "id_maintable" );//document.createElement("table");
					var table = document.getElementById( "id_maintable" );
					var myhead= document.getElementById( "id_myhead" );
					var mybody = document.getElementById( "id_maintbody" );
					var rows = e.target.result.split("\n");
					
					for( var i = 0; i < rows.length; i++) 
					{
						var tr = document.createElement("tr");
						var cells = rows[i].split(",");
						
						if( i == 0 )
						{
							if( cells.length > 1 ) 
							{
								
								//var row = table.insertRow( -1 );

								for( var j = 0; j < cells.length; j++ ) 
								{
									
									var th = document.createElement("th");
									th.setAttribute("class","bg-white text-black dt-head-center");
									th.innerText = cells[j];
									tr.append( th );
									//myhead.append( tr );
	                            }
	                            table.append( tr );
	                            
	                        }
						}
						else
						{
							if( cells.length > 1 ) 
							{
								
								//var row = table.insertRow( -1 );

								for( var j = 0; j < cells.length; j++ ) 
								{
									var td = document.createElement("td");
									
									td.innerText = cells[j];
									tr.append( td );
									//mybody.append( tr );
	                            }
	                            table.append( tr );
	                        }	
						}
					}
				}
			
				reader.readAsText(fileUpload.files[0], "UTF-8");
				
			  
			} 
		}
		
}
*/

