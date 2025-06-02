
$(function()
{
  	var CalibrationMode = sessionStorage.getItem('CalibrationMode');
	
 $(document).ready(function() 
 {
	
	if( CalibrationMode != null )
	{
		if( CalibrationMode > 0  )
		{
			//칼리브레이션 중 이었으면..? 취소 시킴?
			CancleCalibration();
		}	
	}
	
 })
});    
 
 

/*
$(function()
{
 	var button = document.getElementById('id_CalibrationButton');
  	var CalibrationMode = sessionStorage.getItem('CalibrationMode');
	var CalibrationStep = sessionStorage.getItem('CalibrationStep');
	
 $(document).ready(function() 
 {
	
	if( CalibrationMode != null )
	{
		if( CalibrationMode > 0 && CalibrationStep > 0 )
		{
			
			button.disabled = true;
			
			button.classList.remove('btn-primary');
			button.classList.add('btn-secondary');
			
			var progressBar = document.querySelector('.progress-bar');
		    progressBar.style.width = CalibrationStep * 30 + 10 + '%';
		    progressBar.setAttribute('aria-valuenow', CalibrationStep * 30 + 10 );		
		    
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
	
		}	
	}
	
 })
});    
*/ 
 