/**
 * 
 */






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
    history.back();
  }
})
}

function DelTraining(nid)
{
	Swal.fire({
  title: '삭제 하시겠습니까?',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'DeleteTrainingAction.jsp?nid='+nid;
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function DelMap(nid)
{
	Swal.fire({
  title: '삭제 하시겠습니까?',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'DeleteMapAction.jsp?nid='+nid;
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function DelXR(nid)
{
	Swal.fire({
  title: '삭제 하시겠습니까?',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'DeleteXRAction.jsp?nid='+nid;
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}


function DelServer(nid)
{
	Swal.fire({
  title: '삭제 하시겠습니까?',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'DeleteServerAction.jsp?nid='+nid;
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}


function DelAdmin(uid,name)
{
	Swal.fire({
  title: '삭제 하시겠습니까?',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'DeleteAdminAction.jsp?uid='+uid +'&username='+name;
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function DelTrainer(uid,name)
{
	Swal.fire({
  title: '삭제 하시겠습니까?',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'DeleteTrainerAction.jsp?uid='+ uid +'&username='+ name;
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function DelUser(uid,name)
{
	Swal.fire({
  title: '삭제 하시겠습니까?',
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: '삭제',
  
  cancelButtonText: '취소',
  
}).then((result) => {
  /* Read more about isConfirmed, isDenied below */
  if (result.isConfirmed) 
  {
    location.href = 'DeleteUserAction.jsp?uid='+uid +'&username='+name;
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function Main1MessageOK(msg)
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
    location.href = 'admin_main.jsp';
  }
})
}

function Main2MessageOK(msg)
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
    location.href = 'trainer_main.jsp';
  }
})
}

function Main3MessageOK(msg)
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
    location.href = 'user_main.jsp';
  }
})
}

function ServerMessageOK(msg)
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
    location.href = 'server_main.jsp';
  }
})
}

function XRMessageOK(msg)
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
    location.href = 'xr_main.jsp';
  }
})
}

function MapMessageOK(msg)
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
    location.href = 'map_main.jsp';
  }
})
}

function TrainingMessageOK(msg)
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
    location.href = 'training_main.jsp';
  }
})
}



