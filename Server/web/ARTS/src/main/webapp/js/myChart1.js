

var jsonObject1 = JSON.stringify(TrainPerData);
var jData1 = JSON.parse(jsonObject1);
const jDataList1 = jData1["0"]; 
const ValuelList1 = Object.values( jDataList1 ); 

var sum = 0;

for( var i = 0; i <  ValuelList1.length; i++ )
	sum += ValuelList1[i];

const myChart1 = document.getElementById('mychart1').getContext('2d');

const barChart1 = new Chart( myChart1, 
{
        type : 'pie',
        plugins:[ChartDataLabels],
        data :
        {
            
            labels : ['훈련 미수행자','훈련 수행자'],
            datasets : 
            [{
                data : ValuelList1, 
                
                backgroundColor :
                [
                    '#6C757D',
                    '#007BFF'
                    
                ],
                borderWidth : 2,
                borderColor: 
                [
                    '#6C757D',
                    '#007BFF'
                ],
                
            }]
        },
        options :
        {
            layout :
            {
                autoPadding : true,
                padding:
                {
                    left:0,
                    right:0,
                    top:0,
                    bottom:0,
                }
            },
            plugins:  
            {
                datalabels: 
                {
                    color: 'white',
                    
                    formatter: (value)=>
                    {
						let percentage = Math.round( ( value * 100 / sum ) )  +"%";
						return value+'명 : ' + percentage;
					},
                },
                title: 
                {
                    display : true,
                    text : '훈련 수행 비율 (단위:명%)',
                    color:'#fff',
                    align:'center',
                    font :
                    {
                        size:26,
                        weight:'lighter'
                    }
                },
                legend:
                {
                    display : true,
                    position : 'top',
                    labels: 
					{
						color:'rgba(255,255,255,0.9)',
					},
                },
                tooltip :
                {
                    enabled:true,
                },
            },
            
        }
    } ) ;

