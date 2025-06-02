/**
 * 
 */
$(document).ready(function(){

  $('[data-bs-toggle="tooltip"]').tooltip();   

});

function LoginOK(msg)
{
	Swal.fire({
  title: msg,
  icon: 'success',
  showConfirmButton: true,
  confirmButtonColor: '#3085d6',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
    location.href = 'ar_main.jsp';
  }
})
}

function LoginError(msg)
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
   history.back();
  }
})
}

function LogOutMsg(msg)
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
   location.href = 'index.jsp';
  }
})
}

function AddUserOK(msg)
{
	Swal.fire({
  title: msg,
  icon: 'success',
  showConfirmButton: true,
  confirmButtonColor: '#3085d6',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
    location.href = 'ar_user_list.jsp';
  }
})
}

function AddTrainerOK(msg)
{
	Swal.fire({
  title: msg,
  icon: 'success',
  showConfirmButton: true,
  confirmButtonColor: '#3085d6',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
    location.href = 'ar_trainer_list.jsp';
  }
})
}

function MachineMsgOK(msg)
{
	Swal.fire({
  title: msg,
  icon: 'success',
  showConfirmButton: true,
  confirmButtonColor: '#3085d6',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
    location.href = 'ar_mch_list.jsp';
  }
})
}

function MyError(msg)
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
   history.back();
  }
})
}

function MyError3Line(title,titleText,msg)
{
	Swal.fire({
  title: title,

  text: titleText,
  footer:msg,
  icon: 'error',
  showConfirmButton: true,
  confirmButtonColor: '#d33',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
   history.back();
  }
})
}

function MyError2Line(title,msg)
{
	Swal.fire({
  title: title,
  text: msg,
  icon: 'error',
  showConfirmButton: true,
  confirmButtonColor: '#d33',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
   history.back();
  }
})
}

function CancelEditUser()
{
	Swal.fire({
  title: '취소하면 데이터가 수정되지 않습니다.',
  text: '계속 하시겠습니까?',
  icon:'warning',
  width: 800,
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: 'OK',
  
  cancelButtonText: 'Cancel',
  
}).then((result) => {
  
  if (result.isConfirmed) 
  {
	  Swal.fire({
		title:'수정 취소!',
      text:'데이터 수정이 취소 되었습니다',
      icon:'success', 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			   location.href = 'ar_user_list.jsp';
		  }
		  
	  })
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}


function CancelEditTrainer()
{
	Swal.fire({
  title: '취소하면 데이터가 수정되지 않습니다.',
  text: '계속 하시겠습니까?',
  icon:'warning',
  width: 800,
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: 'OK',
  
  cancelButtonText: 'Cancel',
  
}).then((result) => {
  
  if (result.isConfirmed) 
  {
	  Swal.fire({
		title:'수정 취소!',
      text:'데이터 수정이 취소 되었습니다',
      icon:'success', 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			   location.href = 'ar_trainer_list.jsp';
		  }
		  
	  })
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}


function CancelEditMachine()
{
	Swal.fire({
  title: '취소하면 데이터가 수정되지 않습니다.',
  text: '계속 하시겠습니까?',
  icon:'warning',
  width: 800,
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: 'OK',
  
  cancelButtonText: 'Cancel',
  
}).then((result) => {
  
  if (result.isConfirmed) 
  {
	  Swal.fire({
		title:'수정 취소!',
      text:'데이터 수정이 취소 되었습니다',
      icon:'success', 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			   location.href = 'ar_mch_list.jsp';
		  }
		  
	  })
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function CheckPass()
{
	
	var p1 = document.getElementById('pass1').value;
	var p2 = document.getElementById('pass2').value;
	

	if( p1.length < 1 )
	{
		let myform = document.querySelector("form");
		    if( myform.requestSubmit ) 
		    {
	
		        myform.requestSubmit();
		    } 
		    else 
		    {
	
		        myform.submit();
		    }
		return;
	}
	
	if( p1 != p2 )
	{
		Swal.fire({
	  title: '입력한 비밀번호가 일치하지 않습니다. ',
	  icon: 'error',
	  showConfirmButton: true,
	  confirmButtonColor: '#d33',
	  confirmButtonText: '확인' 
		}).then((result) => 
		{
	  		if( result.isConfirmed ) 
	  		{
	   			return false;
	  		}
		})
	}
	else
	{
		
		var inum = document.getElementById('id_num').value;
		
		if( inum.length != 10 )
		{
			Swal.fire({
			  title: '유효하지 않은 군번입니다 ',
			  icon: 'error',
			  showConfirmButton: true,
			  confirmButtonColor: '#d33',
			  confirmButtonText: '확인' 
				}).then((result) => 
				{
			  		if( result.isConfirmed ) 
			  		{
			   			return false;
			  		}
				})
		}
		else
		{
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
		
	}
}


function checkMaxLengthUser(object)
{
	
	let elNumbermessage = document.querySelector('.strongNumber-message');
	var numdiv = document.getElementById('id_numdiv');
	
	if( object.value.length > 0 )
	{
		if( object.value.length >= 10 )
		{
			object.value = object.value.slice(0, 10);
			
			elNumbermessage.classList.add('hide');
			numdiv.classList.remove('mb-3');
			numdiv.classList.add('mb-1');
		}
		//10자리가 안되면
		else
		{
			//유효하지 않는 군번 메세지 출력
			elNumbermessage.classList.remove('hide');
			numdiv.classList.remove('mb-3');
			numdiv.classList.add('mb-1'); 
		}	
	}
	else
	{
		//아무것도 입력 되지 않으면		
		elNumbermessage.classList.add('hide');
		numdiv.classList.remove('mb-3');
		numdiv.classList.add('mb-1');
	}
}

function checkSpace( o )
{
	var str_space = /\s/;	// 공백체크
	
	if( str_space.exec( o.value ) ) 
    { //공백 체크
        
        o.focus();
        o.value = o.value.replace(' ',''); // 공백제거
        return false;
    }
}

function CheckArmyNumber(object)
{
	
	let elNumbermessage = document.querySelector('.strongNumber-message');
	var numdiv = document.getElementById('id_numdiv');
	
	if( object.value.length > 0 )
	{
		if( object.value.length >= 8 )
		{
			object.value = object.value.slice(0, 10);
			
			elNumbermessage.classList.add('hide');
			numdiv.classList.remove('mb-3');
			numdiv.classList.add('mb-1');
		}
		//10자리가 안되면
		else
		{
			//유효하지 않는 군번 메세지 출력
			elNumbermessage.classList.remove('hide');
			numdiv.classList.remove('mb-3');
			numdiv.classList.add('mb-1'); 
		}	
	}
	else
	{
		//아무것도 입력 되지 않으면		
		elNumbermessage.classList.add('hide');
		numdiv.classList.remove('mb-3');
		numdiv.classList.add('mb-1');
	}
}

function checkMaxLength(object)
{
	
	if( object.value.length > object.maxLength )
	{
		 object.value = object.value.slice(0, object.maxLength);
	}
}

function OnChangePass()
{
	var p1 = document.getElementById('pass1');
	let elMismatchMessage = document.querySelector('.mismatch-message');
	var passdiv = document.getElementById('id_passdiv');
	var p2 = $("#pass2");

	if( p1.value.length > 0 )
	{
		p2.attr('required','required');
		p2.removeAttr('disabled');
	}
	else
	{
		p2.val('');
		p2.attr('disabled', 'disabled');
		p2.removeAttr('required','required');
		
		elMismatchMessage.classList.add('hide'); // 실패 메시지가 가려져야 함
    	passdiv.classList.remove('mb-1');
		passdiv.classList.add('mb-3');
		
	}
}

function isMatch (password1, password2) 
{
	return password1 === password2;
}


function MatchCheckPass()  
{

	let elMismatchMessage = document.querySelector('.mismatch-message');
	
	var p1 = document.getElementById('pass1');
	var p2 = document.getElementById('pass2');
	var passdiv = document.getElementById('id_passdiv');
  // console.log(elInputPasswordRetype.value);
  if( p2.value.length > 0 ) 
  {
    if( isMatch( p1.value, p2.value ) ) 
    {
		elMismatchMessage.classList.add('hide'); // 실패 메시지가 가려져야 함
		passdiv.classList.remove('mb-1');
		passdiv.classList.add('mb-3');
    }
    else 
    {
		elMismatchMessage.classList.remove('hide'); // 실패 메시지가 보여야 함
		passdiv.classList.remove('mb-3');
		passdiv.classList.add('mb-1');
    }
  }
  else 
  {
    	elMismatchMessage.classList.add('hide'); // 실패 메시지가 가려져야 함
    	passdiv.classList.remove('mb-1');
		passdiv.classList.add('mb-3');
  }
};

function MatchCheckPassEdit()  
{

	let elMismatchMessage = document.querySelector('.mismatch-message');
	
	var p1 = document.getElementById('pass1');
	var p2 = document.getElementById('pass2');
	var passdiv = document.getElementById('id_passdiv');
  // console.log(elInputPasswordRetype.value);
  if( p2.value.length > 0 ) 
  {
    if( isMatch( p1.value, p2.value ) ) 
    {
		elMismatchMessage.classList.add('hide'); // 실패 메시지가 가려져야 함
		passdiv.classList.remove('mb-1');
		passdiv.classList.add('mb-3-2');
    }
    else 
    {
		elMismatchMessage.classList.remove('hide'); // 실패 메시지가 보여야 함
		passdiv.classList.remove('mb-3-2');
		passdiv.classList.add('mb-1');
    }
  }
  else 
  {
    	elMismatchMessage.classList.add('hide'); // 실패 메시지가 가려져야 함
    	passdiv.classList.remove('mb-1');
		passdiv.classList.add('mb-3-2');
  }
};


function CheckUserNumber()
{
	
	var inum = document.getElementById('id_usernum').value;
	
	if( inum.length != 10 )
	{
		Swal.fire({
		  title: '유효하지 않은 군번입니다 ',
		  icon: 'error',
		  showConfirmButton: true,
		  confirmButtonColor: '#d33',
		  confirmButtonText: '확인' 
			}).then((result) => 
			{
		  		if( result.isConfirmed ) 
		  		{
		   			return false;
		  		}
			})
	}
	else
	{
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
	
	
}
