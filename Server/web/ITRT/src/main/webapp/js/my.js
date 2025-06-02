/**
 * 
 */

$(document).ready(function(){

  $('[data-bs-toggle="tooltip"]').tooltip();   

});

function MyAlert(msg)
{
	Swal.fire({
  icon: 'error',
  title: msg,
  showConfirmButton: false,
  timer: 1000
})
}

function MessageBox(msg)
{
	Swal.fire({
  icon: 'success',
  title: msg,
  showConfirmButton: false,
  timer: 1000
})
}

function UpdateUserMessageOK(msg)
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
    location.href = 'it_user_main.jsp';
  }
})
}

function SetOptionMessageOK(msg,uid)
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
    location.href = 'dr_exec_sim.jsp?uid='+uid;
  }
})
}

function MessageOK(msg)
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
    history.back();
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
   location.href = 'index.jsp';
  }
})
}

function DelUserError(msg)
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
   location.href = 'it_user_main.jsp';
  }
})
}

function AddUserError(msg)
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
   location.href = 'it_add_user.jsp';
  }
})
}


function UpdateUserError(msg, uid)
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
   location.href = 'it_edit_user.jsp?uid='+uid;
  }
})
}


function CancelUpdateUser()
{
	Swal.fire({
  title: 'Cancel하면 작성한 데이터가 수정되지 않습니다.',
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
			   location.href = 'it_user_main.jsp';
		  }
		  
	  })
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function CancelAddUser()
{
	Swal.fire({
  title: 'Cancel하면 작성한 데이터가 저장되지 않습니다.',
  text: "계속 하시겠습니까?",
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
		title:'입력 취소!',
      text:'데이터 입력이 취소 되었습니다',
      icon:'success' 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			   location.href = 'it_user_main.jsp';
		  }
		  
	  })
    
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}



function UserMessageOK(msg)
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
    location.href = 'it_user_main.jsp';
  }
})
}

function UserMessageError(msg)
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
   location.href = 'it_user_main.jsp';
  }
})
}

function OrgMessageOK(msg)
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
    location.href = 'it_add_org.jsp';
  }
})
}

function AddOrgError(msg)
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
   location.href = 'it_add_org.jsp';
  }
})
}

function UpdateOrgMessageOK(msg)
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
    location.href = 'it_org_main.jsp';
  }
})
}

function OrgMessageError(msg)
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
   location.href = 'it_org_main.jsp';
  }
})
}


function UpdateOrgError(msg, oid)
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
   location.href = 'it_edit_org.jsp?uid='+oid;
  }
})
}


function CancelAddOrg()
{
	Swal.fire({
  title: 'Cancel하면 작성한 데이터가 저장되지 않습니다.',
  text: "계속 하시겠습니까?",
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
		title:'입력 취소!',
      text:'데이터 입력이 취소 되었습니다',
      icon:'success' 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			   location.href = 'it_org_main.jsp';
		  }
		  
	  })
    
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}



function AddProdError(msg)
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
   location.href = 'it_add_prod.jsp';
  }
})
}

function CancelAddProd()
{
	Swal.fire({
  title: 'Cancel하면 작성한 데이터가 저장되지 않습니다.',
  text: "계속 하시겠습니까?",
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
		title:'입력 취소!',
      text:'데이터 입력이 취소 되었습니다',
      icon:'success' 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			   location.href = 'it_prod_main.jsp';
		  }
		  
	  })
    
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function UpdateProdMessageOK(msg)
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
    location.href = 'it_prod_main.jsp';
  }
})
}

function UpdateProdError(msg, pid)
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
   location.href = 'it_edit_prod.jsp?uid='+pid;
  }
})
}



function AddUserProdError(msg)
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
   location.href = 'it_add_userprod.jsp';
  }
})
}

function AddUserProdError2(msg, uid)
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
   location.href = 'it_add_userprod2.jsp?uid='+uid;
  }
})
}

function UpdateUserProdMessageOK(msg, uid)
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
    location.href = 'it_view_prod.jsp?uid='+uid;
  }
})
}

function UpdateUserProdError(msg, pid)
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
   location.href = 'it_edit_userprod.jsp?uid='+pid;
  }
})
}

function UpdateUserProdMessageOK2(msg, uid)
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
    location.href = 'it_view_prod.jsp?uid='+uid;
  }
})
}

function UpdateUserProdError2(msg, pid)
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
   location.href = 'it_edit_userprod2.jsp?uid='+pid;
  }
})
}

function AddUserProdMessageOK(msg)
{
	Swal.fire({
  title: msg,
  icon: 'success',
  width: 800,
  showConfirmButton: true,
  confirmButtonColor: '#3085d6',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
    location.href = 'it_userprod_main.jsp';
  }
})
}


function AddUserProdMessageOK2(msg, uid)
{
	Swal.fire({
  title: msg,
  icon: 'success',
  width: 800,
  showConfirmButton: true,
  confirmButtonColor: '#3085d6',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
    location.href = 'it_view_prod.jsp?uid='+uid;
  }
})
}
