/*
$(function()
{
    var $container = $('#rotationSliderContainer');
    var $slider = $('#rotationSlider');
    var $degrees = $('#rotationSliderDegrees');
    
    var sliderWidth = $slider.width();
    var sliderHeight = $slider.height();
    var radius = $container.width()/2;
    var deg = 0;    
    
    X = Math.round(radius* Math.sin(deg*Math.PI/180));
    Y = Math.round(radius*  -Math.cos(deg*Math.PI/180));
    
    $slider.css({ left: X+radius-sliderWidth/2, top: Y+radius-sliderHeight/2 });
    
    var mdown = false;
    $container
    .mousedown(function (e) 
    { 
		mdown = true; e.originalEvent.preventDefault();
		
		    // firefox compatibility
            if(typeof e.offsetX === "undefined" || typeof e.offsetY === "undefined") 
            {
               var targetOffset = $(e.target).offset();
               e.offsetX = e.pageX - targetOffset.left;
               e.offsetY = e.pageY - targetOffset.top;
            }
            
            if($(e.target).is('#rotationSliderContainer'))
            {
				var mPos = {x: e.offsetX, y: e.offsetY};
			}
                
            else
            {
			var mPos = {x: e.target.offsetLeft + e.offsetX, y: e.target.offsetTop + e.offsetY};	
			}
                
                
            var atan = Math.atan2(mPos.x-radius, mPos.y-radius);
            deg = -atan/(Math.PI/180) + 180; // final (0-360 positive) degrees from mouse position 
            
            
            // for attraction to multiple of 90 degrees
            var distance = Math.abs( deg - ( Math.round(deg / 90) * 90 ) );
            
            if( distance <= 5 )
                deg = Math.round(deg / 90) * 90;
                
            if(deg == 360)
                deg = 0;
            
            X = Math.round(radius* Math.sin(deg*Math.PI/180));
            Y = Math.round(radius*  -Math.cos(deg*Math.PI/180));

            $slider.css({ left: X+radius-sliderWidth/2, top: Y+radius-sliderHeight/2 });              
            
            
            var roundDeg = Math.round(deg);
            
            $slider.css("transform", "rotate(" + deg + "deg)");
            
            $degrees.html(roundDeg + '&deg;');
            $('#imageRotateDegrees').val(roundDeg); 
    })
    .mouseup(function (e) { mdown = false; })
    .mousemove(function (e) 
    {
        if( mdown )
        {
            
            // firefox compatibility
            if(typeof e.offsetX === "undefined" || typeof e.offsetY === "undefined") 
            {
               var targetOffset = $(e.target).offset();
               e.offsetX = e.pageX - targetOffset.left;
               e.offsetY = e.pageY - targetOffset.top;
            }
            
            if($(e.target).is('#rotationSliderContainer'))
            {
                var mPos = {x: e.offsetX, y: e.offsetY};
                
            }
            else
            {
				var mPos = {x: e.target.offsetLeft + e.offsetX, y: e.target.offsetTop + e.offsetY};
				
			}
                
                
            var atan = Math.atan2(mPos.x-radius, mPos.y-radius);
            deg = -atan/(Math.PI/180) + 180; // final (0-360 positive) degrees from mouse position 
            
            
            // for attraction to multiple of 90 degrees
            var distance = Math.abs( deg - ( Math.round(deg / 90) * 90 ) );
            
            if( distance <= 5 )
                deg = Math.round(deg / 90) * 90;
                
            if(deg == 360)
                deg = 0;
            
            X = Math.round(radius* Math.sin(deg*Math.PI/180));
            Y = Math.round(radius*  -Math.cos(deg*Math.PI/180));

            $slider.css({ left: X+radius-sliderWidth/2, top: Y+radius-sliderHeight/2 });              
            
            var roundDeg = Math.round(deg);
            
            $slider.css("transform", "rotate(" + roundDeg + "deg)");
            
            $degrees.html(roundDeg + '&deg;');
            $('#imageRotateDegrees').val(roundDeg);
                
        }
    });

});
*/    


 $(function()
{
    var $container = $('#circle');
    var $slider = $('#arrow');
    var $degrees = $('#IDEditWindPos');
    
    
    
    var sliderWidth = $slider.width();
    var sliderHeight = $slider.height();
    var radius = $container.width()/2;
    var deg = 0;    
    
    X = Math.round( radius * Math.sin( deg * Math.PI / 180 ) );
    Y = Math.round( radius *  -Math.cos( deg * Math.PI / 180 ) );
    
    $slider.css({ left: X + radius - sliderWidth / 2, top: Y + radius - sliderHeight / 2  });
  
 
    var mdown = false;

    $container
    .mousedown(function (e) 
    { 
		mdown = true;
		
		    // firefox compatibility
            if(typeof e.offsetX === "undefined" || typeof e.offsetY === "undefined") 
            {
               var targetOffset = $(e.target).offset();
               e.offsetX = e.pageX - targetOffset.left;
               e.offsetY = e.pageY - targetOffset.top;
               
            }
            
            
            
            var mPos;
            if($(e.target).is('#circle'))
            {
				var mPos = {x: e.offsetX, y: e.offsetY};
			}
                 
            else
            {
				 var mPos = {x: e.target.offsetLeft + e.offsetX, y: e.target.offsetTop + e.offsetY};
			}
                
                
            var atan = Math.atan2( mPos.x - radius, mPos.y - radius );
            deg = -atan/(Math.PI/180) + 180; 
            
            
            var distance = Math.abs( deg - ( Math.round( deg / 90 ) * 90 ) );
            
            if( distance <= 5 )
                deg = Math.round(deg / 90) * 90;
                
            if(deg == 360)
                deg = 0;
            
            X = Math.round( radius * Math.sin( deg * Math.PI / 180 ) );
            Y = Math.round( radius *  -Math.cos( deg * Math.PI / 180 ) );

            $slider.css({ left: X + radius - sliderWidth / 2, top: Y + radius - sliderHeight / 2  });         
            
            
            var roundDeg = Math.round( deg );
            
            $slider.css("transform", "rotate(" + roundDeg + "deg)");
            
            $degrees.val(roundDeg + '°');
            //$degrees.html(roundDeg + '&deg;');
            //$('#debug').val(roundDeg); 
            
            
            
             
    })
    .mouseup(function (e) 
    { 
		mdown = false;

	})
    .mousemove(function (e) 
    {
        if( mdown )
        {
            
            // firefox compatibility
            if(typeof e.offsetX === "undefined" || typeof e.offsetY === "undefined") 
            {
               var targetOffset = $(e.target).offset();
               e.offsetX = e.pageX - targetOffset.left;
               e.offsetY = e.pageY - targetOffset.top;
               
               mdown = false;
            }
            
            if($(e.target).is('#circle'))
                var mPos = {x: e.offsetX, y: e.offsetY};
            else
                var mPos = {x: e.target.offsetLeft + e.offsetX, y: e.target.offsetTop + e.offsetY};
                
            var atan = Math.atan2( mPos.x - radius, mPos.y - radius );
            deg = -atan /( Math.PI / 180 ) + 180; // final (0-360 positive) degrees from mouse position 
            
            
            // for attraction to multiple of 90 degrees
            var distance = Math.abs( deg - ( Math.round( deg / 90 ) * 90 ) );
            
            if( distance <= 5 )
                deg = Math.round( deg / 90 ) * 90;
                
            if(deg == 360)
                deg = 0;
            
            X = Math.round( radius * Math.sin( deg * Math.PI / 180 ) );
            Y = Math.round( radius *  -Math.cos( deg * Math.PI / 180 ) );
			
			
			
            $slider.css({ left: X + radius - sliderWidth / 2, top: Y + radius - sliderHeight / 2  });
            
            var roundDeg = Math.round(deg);
            
            $slider.css("transform", "rotate(" + roundDeg + "deg)");
            
            $degrees.val(roundDeg + '°');
          //  $('#debug').val(roundDeg);
                
        }
    });
 $(document).ready(function() 
 {
	 $(document).on("mouseup touchend", function(e) 
    {
      return mdown = false;
    });
 })
});    
 
 