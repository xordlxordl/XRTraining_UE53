	
	function changeCityFn()
	{
		var city  = document.getElementById("city");
		var selectedindex = city.selectedIndex;
		
		if( selectedindex <= 0 )
		{
			var accumulateGun = $('#gungu');
			accumulateGun.empty();
			accumulateGun.append('<option value="">시/군/구</option>');
			
			var accumulatetDong = $('#dong');
			accumulatetDong.empty();
			accumulatetDong.append('<option value="">읍/면/동</option>');
	
			return;
		}
		
		var JsonInfo;


		 
		$.ajax({
		url: './dr_SelectSiDo.jsp',
		type:'GET',
		dataType: 'json',
		data:{"sido":selectedindex},
		async: false,
		contentType: 'application/json; charset=utf-8',
		success : function(request)
		{
		 JsonInfo =  request;
		}});
		  
		console.log("JsonInfo : " +JsonInfo);

		var accumulateEvent = $('#gungu');
		accumulateEvent.empty();
		
		accumulateEvent.append('<option value="">시/군/구</option>');
		var accumulateUl;
		
		var accumulateEventDong = $('#dong');
		accumulateEventDong.empty();
		accumulateEventDong.append('<option value="">읍/면/동</option>');
		
		
		if( JsonInfo.length == 1 )
		{
			if( JsonInfo[0].nid == 114 )
			{
				
				accumulateUl = "<option selected value='" + JsonInfo[0].nid + "'>" + JsonInfo[0].szname + "</option>";
				accumulateEvent.append( accumulateUl );
				changeSejong();
				return;
			}
			
		}

		
		for( var i = 0; i < JsonInfo.length; ++i ) 
		{
			accumulateUl = "<option value='" + JsonInfo[i].nid + "'>" + JsonInfo[i].szname + "</option>";
			accumulateEvent.append( accumulateUl );
		}
		
	};
	
		
	function changeSejong()
	{
		
		var JsonInfo;


		 
		$.ajax({
		url: './dr_SelectGunGu.jsp',
		type:'GET',
		dataType: 'json',
		data:{"gungu":114},
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