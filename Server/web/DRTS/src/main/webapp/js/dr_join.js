/**
 * 
 */


$('.datepicker').datepicker({
	        
        startView: 2,
        language: "ko"
	        
});

$('input[name="bcrash"]').change( function()
{
	var value = $(this).val();
	var checked = $(this).prop('checked');
	if( value == "1")
	{
		
		document.getElementById('crash_option').classList.remove('text-bg-secondary');
		
		
		document.getElementById('crash_text').classList.remove('text-opacity-50');
		document.getElementById('crash_text').classList.remove('text-light');
		document.getElementById('crash_text').classList.add('text-dark');
		
		
		document.getElementById('inlineCheckCrashObject').disabled = false;
		document.getElementById('inlineCheckCrashHuman').disabled = false;
		document.getElementById('inlineCheckCrashOnly').disabled = false;
				
	}
	else
	{
		//document.getElementById('crash_option').classList.add('visually-hidden-focusable');
		//$('#crash_option').css('display','none');
		
		
		document.getElementById('crash_option').classList.add('text-bg-secondary');
		
		document.getElementById('crash_text').classList.add('text-opacity-50');
		document.getElementById('crash_text').classList.add('text-light');
		document.getElementById('crash_text').classList.remove('text-dark');
		
		document.getElementById('inlineCheckCrashObject').disabled = true;
		document.getElementById('inlineCheckCrashHuman').disabled = true;
		document.getElementById('inlineCheckCrashOnly').disabled = true;
		
		document.getElementById('inlineCheckCrashObject').checked = false;
		document.getElementById('inlineCheckCrashHuman').checked = false;
		document.getElementById('inlineCheckCrashOnly').checked = false;

	}
});

		$('input[id="birthdatepicker"]').change( function()
		{
			var strbirth = $('#birthdatepicker').val();	
			
			var age = getWesternAge(strbirth);
			$('#age').val(age);
			
		});
		function getWesternAge(birthday) {
			let today = new Date();
			let birthDay = new Date(birthday);
			let age = today.getFullYear() - birthDay.getFullYear();
			
			let todayMonth = today.getMonth() + 1;
			let birthMonth = birthDay.getMonth() + 1;
			
			if (birthMonth > todayMonth || (birthMonth === todayMonth && birthDay.getDate() >= today.getDate())) {
				age--;
			} 
			return age;
		}
		


$('input[name="bsick"]').change( function()
		{
			var value = $(this).val();
			if( value == "1" )
			{
				
				document.getElementById('sicktext').disabled = false;
						
			}
			else
			{
				document.getElementById('sicktext').disabled = true;
			}
		});





$('input[name="bdrug"]').change( function()
		{
			var value = $(this).val();
			if( value == "1" )
			{
				
				document.getElementById('drugtext').disabled = false;
						
			}
			else
			{
				document.getElementById('drugtext').disabled = true;
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
