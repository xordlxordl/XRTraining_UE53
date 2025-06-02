/**
 * 
 */
	//프로토콜 단순 정의 T: Trainer U:User
	const enProtocol =
	{
		 enCloseSocket : -99
		,enError : -100
		
		,enEndSetOptionReq : 1			//T -> U
		,enEndSetOptionAck : 2			//U -> T
		,enSendRestart : 3				//T -> U
		,enSendReset : 4				//T -> U
		,enCalibrationStartReq : 5		//T -> U CalibrationReady
		,enCalibrationStartAck : 6		//U -> T
		,enCalibrationStep1Req : 7		//T -> U
		,enCalibrationStep1Ack : 8		//U -> T
		,enCalibrationStep2Req : 9		//T -> U
		,enCalibrationStep2Ack : 10		//T -> U
		,enCalibrationStep3Req : 11		//T -> U
		,enCalibrationStep3Ack : 12		//U -> T
		,enCalibrationEnd : 13			//T -> U
		,enCalibrationCancle : 14		//T -> U
	};


      //var webSocket = new WebSocket("ws://192.168.0.64:8080/DRTS/DRServer");
      
      var webSocket = new WebSocket("ws://localhost:8080/DRTS/DRServer");
      //var webSocket = new WebSocket("ws://192.168.0.205:8080/DRTS/DRServer");
      var messageTextArea = document.getElementById("messageTextArea");
      
      // WebSocket 서버와 접속이 되면 호출되는 함수
      webSocket.onopen = function(message) 
      {
        console.log("Server connect...\n");
      };
      
      // WebSocket 서버와 접속이 끊기면 호출되는 함수
      webSocket.onclose = function(message) 
      {
        
        
        console.log("Server Disconnect...\n");
      };
      
      // WebSocket 서버와 통신 중에 에러가 발생하면 요청되는 함수
      webSocket.onerror = function(message) 
      {
      
        console.log("error...\n");
        
      };
      
      // WebSocket 서버로 부터 메시지가 오면 호출되는 함수
      webSocket.onmessage = function(message) 
      {
        console.log( message.data+"\n");
        
        
        const obj = JSON.parse( message.data );
        
        if( obj == false )
        {
			return;
		}
        
        var protocol = parseInt( obj.p, 10 );
        
        switch( protocol )
        {
			case enProtocol.enEndSetOptionReq: 			SendEndOption( obj );	break;
			case enProtocol.enEndSetOptionAck:			RecvEndOption();		break;
			case enProtocol.enSendRestart:				RecvRestart();			break;
			case enProtocol.enSendReset:				RecvReset();			break;
			case enProtocol.enCalibrationStartReq:		RecvCalibrationStart();	break;	//enCalibrationStart
			case enProtocol.enCalibrationStartAck:		RecvCalibrationReady();	break;	//Rcv Trainer
			case enProtocol.enCalibrationStep1Req:		RecvCalibrationStep1();	break;
			case enProtocol.enCalibrationStep1Ack:		RecvCalibrationStep2();	break;
			case enProtocol.enCalibrationStep2Req:		RecvCalibrationEnd();	break;
			case enProtocol.enCloseSocket:				RecvOnClose( obj );		break;
			case enProtocol.enError:					RecvError();			break;
		}
        
      };

     // Send 버튼을 누르면 호출되는 함수
	function sendMessage()
     {
      // 유저명 텍스트 박스 오브젝트를 취득
      var user = document.getElementById("user");
      // 송신 메시지를 작성하는 텍스트 박스 오브젝트를 취득
      var message = document.getElementById("textMessage");
      // 콘솔 텍스트에 메시지를 출력한다.
      messageTextArea.value += user.value + "(me) => " + message.value + "\n";
      // WebSocket 서버에 메시지를 전송(형식 「{{유저명}}메시지」)
      webSocket.send("{{" + user.value + "}}" + message.value);
      // 송신 메시지를 작성한 텍스트 박스를 초기화한다.
      message.value = "";
    }

	//function SendSetOption( uid, path, bAEBS, area, timezone, weather, amount, windspeed, winddir )
	//function SendSetOption( uid, mode, bAEBS, timezone, weather, amount, windspeed, winddir )
	function SendSetOption()
	{
		var sendobj = new Object();
		
		
		var uidlist = $('input[name="uid"]');
		var uid = uidlist[0].value;
		
		
		
		var modelist = $('input[name="mode"]')
		
		var mode;
		
		var i;
		for( i = 0; i < modelist.length; ++i )
		{
			if( modelist[i].checked == true )
			{
				
				mode = modelist[i].value;
				break;
			}
			
		}
		/*
		if( mode == null )
		{
			MyAlert("모드 종류를 선택하세요");
			return;
		}
		*/
		/*
		//패쓰 없어짐
		var pathlist = $('input[name="path"]')
		
		var path;
		
		var i;
		for( i = 0; i < pathlist.length; ++i )
		{
			if( pathlist[i].checked == true )
			{
				
				path = pathlist[i].value;
				break;
			}
			
		}
		*/
		
		var bAEBSlist = $('input[name="bAEBS"]')
		var bAEBS = "0";
		if( bAEBSlist[0].checked == true )
			bAEBS = "1";
		
		
		/*
		//지역 사라림
		var arealist = $('input[name="area"]');
		
		var area;
		for( i = 0; i < arealist.length; ++i )
		{
			if( arealist[i].checked == true )
			{
				
				area = arealist[i].value;
				break;
			}
		}
		*/
	
		var timezonelist = $('input[name="timezone"]');
		var timezone;
		
		for( i = 0; i < timezonelist.length; ++i )
		{
			if( timezonelist[i].checked == true )
			{
				
				timezone = timezonelist[i].value;
				break;
			}
		}

		
		var weatherlist = $('input[name="weather"]');
		var weather;
		for( i = 0; i < weatherlist.length; ++i )
		{
			if( weatherlist[i].checked == true )
			{
				
				weather = weatherlist[i].value;
				break;
			}
		}
		
		var amountlist = $('input[name="amount"]');
		var amount = amountlist[0].value;
		if( amount == null )
			amount = 0;
		
		
		var windspeedlist = $('input[name="windspeed"]');
		var windspeed = windspeedlist[0].value;
		
		
		var winddirlist = $('input[name="winddir"]');
		
		const regex = /[^0-9]/g;
		
		var winddir =winddirlist[0].value.replace(regex, "");
		
		sendobj.p = enProtocol.enEndSetOptionReq;//"1";
		sendobj.uid = uid;
		sendobj.mode = mode;
		sendobj.bAEBS = bAEBS;
		
		//sendobj.path = path;
		//sendobj.area = area;
		
		sendobj.timezone = timezone;
		sendobj.weather = weather;
		sendobj.amount = amount;
		sendobj.windspeed = windspeed;
		sendobj.winddir = winddir;
		
		console.log( sendobj.toString() );
		
      	var jsonstr = JSON.stringify( sendobj );
      
      
		webSocket.send( jsonstr );
		console.log( jsonstr );
      
    }
    
    //클라에서 세팅을 다 한 후 클라 세팅이 끝났음을 관제로 전송
    function SendEndOption(obj)
    {
		var sendobj = new Object();
		sendobj.p = "2";
		sendobj.uid = obj.uid;
		var jsonstr = JSON.stringify( sendobj );


      	messageTextArea.value += ( JSON.stringify( obj ) + "\n" );
      	
      	messageTextArea.value += ( "SendEndOption........." + "\n");
      	
      	
		webSocket.send( jsonstr );
		console.log( jsonstr );
	}
	
	//관제 페이지에서 클라 세팅이 완료 됬음을 리시브 함
    function RecvEndOption()
    {
		//모든 컨트롤 값이 정상이면 submit을 실행
		//값이 비정상이면 dr_UpdateSimAction.jsp은 호출 되지 않음
		//dr_UpdateSimAction.jsp에서 호출 하는 DB 호출도 되지 않는다.
		
		let myform = document.querySelector("form");
	    if( myform.requestSubmit ) 
	    {

	        myform.requestSubmit();
	    } 
	    else 
	    {

	        myform.submit();
	    }

		console.log( jsonstr );
	}
	
	
	function SendRestart()
    {
		var sendobj = new Object();
		sendobj.p = "3";
		
		var jsonstr = JSON.stringify( sendobj );
      	
		webSocket.send( jsonstr );
		console.log( jsonstr );
	}
	
	function RecvRestart()
    {
		messageTextArea.value += "RecvRestart............\n";
	}
	
	
	function SendReset()
    {
		var sendobj = new Object();
		sendobj.p = "4";
		
		var jsonstr = JSON.stringify( sendobj );
      	
      	sessionStorage.clear();
		webSocket.send( jsonstr );
		console.log( jsonstr );
	}
	
	function RecvReset()
    {
		messageTextArea.value += "RecvReset............\n";
	}	
    function RecvOnClose(obj)
    {
		messageTextArea.value +=  ( JSON.stringify( obj ) + "\n" );
	}
	
	function SendCalibrationStart()
    {
		var sendobj = new Object();
		sendobj.p = "5";	//enCalibrationStart
		
		var jsonstr = JSON.stringify( sendobj );
      	
		webSocket.send( jsonstr );
		console.log( jsonstr );
		
		//
	}
	
	function RecvCalibrationStart()
    {
		SendCalibrationStep1();
	}
	
	//User -> Trainer
	function SendCalibrationStep1()
    {
		var sendobj = new Object();
		sendobj.p = "6";
		
		var jsonstr = JSON.stringify( sendobj );
      	
		webSocket.send( jsonstr );
		messageTextArea.value += (jsonstr + "\n");
		console.log( jsonstr );
		//
	}
	//시뮬레이터가 패킷을 받으면 패킷을 받았다는 패킷( 즉 스텝1이 되면 버튼 변경)	
	function RecvCalibrationReady()
    {
		var button = document.getElementById('id_CalibrationButton');
		
		button.disabled = true;
	
		button.classList.remove('btn-primary');
		button.classList.add('btn-secondary');
		
		var button_ok = document.getElementById('id_simsetbutton_ok');
		
		button_ok.disabled = true;
		
		button_ok.classList.remove('btn-primary');
		button_ok.classList.add('btn-secondary');
		
		
		document.getElementById('id_practice_mode').checked = false;
		
		document.getElementById('id_driving_mode').checked = false;
		
		
		//세션
		sessionStorage.setItem('CalibrationMode', '1');
		sessionStorage.setItem('CalibrationStep', '0');

		var disabledRadioButtons = $(".dr_class_driving_mode");
		disabledRadioButtons.attr('disabled', 'disabled');
		
		var disabledMode = $(".dr_nocali");
		disabledMode.attr('disabled', 'disabled');
		
		//써클 초기화		
		
		var $circle1 = $('#circle1');
		var $slider = $('#arrow');
		
		var $container = $('#circle');
		var $degrees = $('#IDEditWindPos');
    
		var sliderWidth = $slider.width();
		var sliderHeight = $slider.height();
		var radius = $container.width()/2;
		var deg = 0;    
		
		X = Math.round( radius * Math.sin( deg * Math.PI / 180 ) );
		Y = Math.round( radius *  -Math.cos( deg * Math.PI / 180 ) );
		
		$slider.css({ left: X + radius - sliderWidth / 2, top: Y + radius - sliderHeight / 2  });
		var roundDeg = Math.round( deg );
            
		$slider.css("transform", "rotate(" + roundDeg + "deg)");
            
		$degrees.val(roundDeg + '°');
		
		
		$slider.css("color","#ced4da" );
		$circle1.css("background","#dee2e6" );
		
		var NextButton = document.getElementById('id_NextButton');
		
		NextButton.style.display = 'block';
		NextButton.value = '1단계';
		
	}
	
	//Trainer -> User 
	function RecvCalibrationStep1()
    {
		
		var progressBar = document.querySelector('.progress-bar');
        progressBar.style.width = 40 + '%';
        progressBar.setAttribute('aria-valuenow', 40);
		//
		
		//세션
		sessionStorage.setItem('CalibrationMode', '1');
		sessionStorage.setItem('CalibrationStep', '1');
			
		var progressBar = document.querySelector('.progress-bar');
        progressBar.style.width = 40 + '%';
        progressBar.setAttribute('aria-valuenow', 40);
        
		var NextButton = document.getElementById('id_NextButton');
		
		NextButton.style.display = 'block';
		NextButton.value = '1단계';
		
	}
	
	function SendCalibrationNextStep()
    {
		sessionStorage.clear();
		
		var sendobj = new Object();
		
		var CalibrationStep = sessionStorage.getItem('CalibrationStep');
		
		if( CalibrationStep == 0 )
			sendobj.p = "7";
		else if( CalibrationStep == 1 )
			sendobj.p = "8";	
		else if( CalibrationStep == 2 )
			sendobj.p = "9";
		
		var jsonstr = JSON.stringify( sendobj );
      	
		webSocket.send( jsonstr );
		
		console.log( jsonstr );
	}
	
	function SendCalibrationCancel()
    {
		sessionStorage.clear();
		
		var sendobj = new Object();
		sendobj.p = "11";
		
		var jsonstr = JSON.stringify( sendobj );
      	
		webSocket.send( jsonstr );
		
		console.log( jsonstr );
	}
	
	//Trainer 
	function SendCalibrationEnd()
    {
		sessionStorage.clear();
		
		var sendobj = new Object();
		sendobj.p = "10";
		
		var jsonstr = JSON.stringify( sendobj );
      	
		webSocket.send( jsonstr );
		
		console.log( jsonstr );
	}
	
	//User
	function RecvCalibrationEnd()
    {
		
		messageTextArea.value += ("RecvCalibrationEnd......" + "\n");
		console.log( messageTextArea.value );
	}

	
    function RecvError()
    {
		MyAlert("접속한 클라이언트가 없습니다");
	}

	
	
    // Disconnect 버튼을 누르면 호출되는 함수
    function disconnect() 
    {
      // WebSocket 접속 해제
      webSocket.close();
    }