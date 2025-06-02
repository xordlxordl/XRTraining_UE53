

 $(function()
{
    var $container = $('#circle');
    var $slider = $('#arrow');
    var $degrees = $('#IDEditWindPos');
    
    
    
    
    var sliderWidth = $slider.width();
    var sliderHeight = $slider.height();
    var radius = $container.width()/2;
    var deg = $degrees.val();    
   

 
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
    
            
 
});    
 
 