



var jsonObject3 = JSON.stringify(TrainCountArray);

var jData3 = JSON.parse(jsonObject3);

var LabelList3 = new Array();
var DataList3 = new Array();

var iMaxCount3 = 0;
var iCurCount3 = 0;

for(var i = 0; i< jData3.length; i++) 
{
	var d = jData3[i];
	LabelList3.push( d.tname );
	DataList3.push( d.ncount )
	
	iCurCount3 = d.ncount;
	
	if( iCurCount3 > iMaxCount3 )
		iMaxCount3 = iCurCount3;

}


const myChart3 = document.getElementById('mychart3').getContext('2d');
const canvas = document.getElementById('mychart3');


 var DATA_COUNT3 = jData3.length;
 
 canvas.height = DATA_COUNT3 * 20;
  
  const labels3 = LabelList3;// ['기지방호', '독수리', '수류탄투척', '사격', '전투준비'];
  const data3 =
  {
    
    labels: labels3,
    datasets: 
    [
      {
        label: '수행 횟수',
        data: DataList3,
        borderColor: 
        [
            '#28A745',
            
        ],
        backgroundColor: 
        [
            '#28A745',
            
        ],
        borderWidth: 0,
        borderRadius: Number.MAX_VALUE,
        borderSkipped: false,
        
      },
    ],
    
  };
  // </block:setup>
  const scale3 = 
	{
		x: 
		{
			ticks:
		  	{
				color:'rgba(255,255,255,0.9)',
				weight:'lighter'
		  	}
		},
		y: 
		{
			ticks:
		  	{
				color:'rgba(255,255,255,0.9)',
				weight:'lighter'
		  	}
		}
		  
	} 	

  
  // <block:config:0>
  const config3 = 
  {
    plugins:[ChartDataLabels],
    type: 'bar',
    data: data3,
    options: 
    {
		indexAxis:'y',
       
		responsive: true,
		maintainAspectRatio: false,
		 scales: scale3,
      plugins:
      {
		  legend: 
		 {
			labels: 
			{
				color:'rgba(255,255,255,0.9)',
			},
		},
        datalabels: 
        {
            color: '#fff',
            
        },
       
        title: 
        {
            display : true,
            text : '시나리오별 수행 횟수(단위:번)',
            color:'#fff',
            align:'center',
            
            font :
            {
                size:26,
                weight:'lighter'
            }
        },
      
      }
    },
  };

  const barChart3 = new Chart( myChart3,  config3 );
  
  