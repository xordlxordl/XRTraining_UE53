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
    location.href = 'dr_user_main.jsp';
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
			   location.href = 'dr_user_main.jsp';
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
			   location.href = 'dr_user_main.jsp';
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
    location.href = 'dr_user_main.jsp';
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
   location.href = 'dr_user_main.jsp';
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

function RestartPracticeMode()
{
	Swal.fire({
  title: '연습모드 재실행',
  text: "재실행 하시겠습니까?",
  icon:'warning',
  
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: 'OK',
  
  cancelButtonText: 'Cancel',
  
}).then((result) => {
  
  if (result.isConfirmed) 
  {
	  Swal.fire({
		title:'재실행!',
      text:'OK 버튼을 누르면 연습모드를 재실행 합니다.',
      icon:'success' 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			 SendRestart();
		  }
		  
	  })
    
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function ResetDrivingMode(uid)
{
	Swal.fire({
  title: '주행 모드 리셋',
  text: "리셋 하시겠습니까?",
  icon:'warning',
  
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: 'OK',
  
  cancelButtonText: 'Cancel',
  
}).then((result) => {
  
  if (result.isConfirmed) 
  {
	  Swal.fire({
		title:'리셋!',
      text:'OK 버튼을 누르면 주행모드를 리셋 합니다.',
      icon:'success' 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			 SendReset();
			 location.href = 'dr_view_sim.jsp?uid='+uid;
			 
		  }
		  
	  })
    
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}


function CancleSimSet()
{
	Swal.fire({
  title: '환경 설정 취소',
  text: " 취소 하시겠습니까?",
  icon:'warning',
  
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: 'OK',
  
  cancelButtonText: 'Cancel',
  
}).then((result) => {
  
  if (result.isConfirmed) 
  {
	  Swal.fire({
		title:'취소!',
      text:'환경 설정을 취소합니다.',
      icon:'success' 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			 SendReset();
			 location.href = 'dr_user_main.jsp';
			 
		  }
		  
	  })
    
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function StartCalibration()
{
	Swal.fire({
  title: 'Calibration',
  text: "Calibration을 실행합니다",
  icon:'info',
  
  showConfirmButton: true,
  
  showCancelButton: true,
  confirmButtonText: 'OK',
  
  cancelButtonText: 'Cancel',
  
}).then((result) => {
  
  if (result.isConfirmed) 
  {
	  Swal.fire({
		title:'Calibration',
      text:'Calibration을 실행합니다.',
      icon:'success' 
      }).then((result)  =>
      {
		  if( result.isConfirmed)
		  {
			 SendCalibrationStart();
		  }
		  
	  })
    
  }
  else
  {
    Swal.fire('취소하셨습니다', '', 'info')
  }
})
}

function CancleCalibration()
{
	Swal.fire({
  title: 'Calibration이 취소되었습니다!',
  icon: 'error',
  showConfirmButton: true,
  confirmButtonColor: '#d33',
  confirmButtonText: '확인'
}).then((result) => 
{
  if( result.isConfirmed ) 
  {
	SendCalibrationCancel();
//	location.href = 'dr_user_main.jsp';
  }
})
}


