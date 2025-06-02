/**
 * 
 */

$('input[name="mode"]').change( function()
{
	var value = $(this).val();
	
	var disabledRadioButtons = $(".dr_class_driving_mode");
	
	var $circle1 = $('#circle1');
	var $slider = $('#arrow');
	if( value == "1")
	{
		disabledRadioButtons.attr('disabled', 'disabled');
		
		//체크 해제
		
		document.getElementById('idtimeam').checked = false;
		document.getElementById('idtimepm').checked = false;
		document.getElementById('idtimeevening').checked = false;
		document.getElementById('idtimenight').checked = false;
		
		document.getElementById('idweathersun').checked = false;
		document.getElementById('idweatherrain').checked = false;
		document.getElementById('idweathersnow').checked = false;
		document.getElementById('idweatherfog').checked = false;
		
		document.getElementById('idaebs').checked = false;
		
		document.getElementById('IDRangeAmount').value = 0;
		document.getElementById('IDRangeWind').value = 0;
		
		document.getElementById('IDEditAmouont').value = 0;
		document.getElementById('IDEditWind').value = 0;
		

		document.getElementById('IDRangeAmount').disabled = true;
		document.getElementById('IDRangeWind').disabled = true;
		
		document.getElementById('IDEditAmouont').disabled = true;
		document.getElementById('IDEditWind').disabled = true;
		
		//써클 초기화		
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
		//background: #9b59b6;
  
	
		
	}
	else
	{
		disabledRadioButtons.removeAttr('disabled');
		
		$slider.css("color","#F00" );
		$circle1.css("background","#0d6efd" );
		
		//document.getElementById('idcityarea').checked = false;
		//document.getElementById('idhigharea').checked = false;
		//document.getElementById('idlowarea').checked = false;

	}
});


$('input[name="weather"]').change( function()
{
	var value = $(this).val();
	
	
	if( value == "1")
	{
		document.getElementById('IDRangeAmount').disabled = true;
		document.getElementById('IDEditAmouont').disabled = true;
	}
	else
	{

		document.getElementById('IDRangeAmount').disabled = false;
		document.getElementById('IDEditAmouont').disabled = false;
	}
});


$(function()
{
    var requiredCheckboxes = $(".dr_checkbox");
    requiredCheckboxes.change(function()
	{
        if(requiredCheckboxes.is(':checked')) 
        {
            requiredCheckboxes.removeAttr('required');
        }
        else 
        {
            requiredCheckboxes.attr('required', 'required');
        }
    });
});
