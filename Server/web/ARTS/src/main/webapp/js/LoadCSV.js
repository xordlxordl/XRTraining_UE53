/**
 * 
 */
  function loadcsv() 
  {
  
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
									myhead.append( tr );
	                            }
	                            
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
									mybody.append( tr );
	                            }
	                        }	
						}
					}
				}
				
			reader.readAsText(fileUpload.files[0], "UTF-8");
			
			} 
			else
			{
                    alert("This browser does not support HTML5.");
			}
		}
		else
		{
                alert("Please upload a valid CSV file.");
		}
}
