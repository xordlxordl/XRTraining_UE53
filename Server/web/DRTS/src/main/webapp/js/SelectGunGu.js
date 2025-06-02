	
	function changeGunGuFn()
	{
		var gungu  = document.getElementById("gungu");
		var selectedindex = gungu.selectedIndex;
		
		console.log("gungu : " + selectedindex);
		
		if( selectedindex <= 0 )
		{
			var accumulatetDong = $('#dong');
			accumulatetDong.empty();
			accumulatetDong.append('<option value="">읍/면/동</option>');
			return;	
		}
		
		
		var optvalue = gungu.options[selectedindex].value;
		
		
		
		var JsonInfo;


		 
		$.ajax({
		url: './dr_SelectGunGu.jsp',
		type:'GET',
		dataType: 'json',
		data:{"gungu":optvalue},
		async: false,
		contentType: 'application/json; charset=utf-8',
		success : function(request)
		{
		 JsonInfo =  request;
		}});
		  
		console.log("JsonInfo : " + JsonInfo);

		var accumulateEvent = $('#dong');
		accumulateEvent.empty();
		accumulateEvent.append('<option value="">읍/면/동</option>');
		var accumulateUl;
		
		
		for( var i = 0; i < JsonInfo.length; ++i ) 
		{
			accumulateUl = "<option value='" + JsonInfo[i].nid + "'>" + JsonInfo[i].szname + "</option>";
			accumulateEvent.append( accumulateUl );
		}
		
	};