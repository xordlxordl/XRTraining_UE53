



var jsonObject2 = JSON.stringify(TrainTimeArray);

var jData2 = JSON.parse(jsonObject2);

var LabelList2 = new Array();
var DataList2 = new Array();

var iMaxCount2 = 0;
var iCurCount2 = 0;

for(var i = 0; i< jData2.length; i++) 
{
	var d = jData2[i];
	LabelList2.push( d.name );
	DataList2.push( d.min )
	
	iCurCount2 = d.ncount;
	
	if( iCurCount2 > iMaxCount2 )
		iMaxCount2 = iCurCount2;

}


const myChart2 = document.getElementById('mychart2').getContext('2d');
const canvas2 = document.getElementById('mychart2');


 var DATA_COUNT2 = jData2.length;
 
 canvas2.height = DATA_COUNT2 * 20;
  
  const labels2 = LabelList2;// ['기지방호', '독수리', '수류탄투척', '사격', '전투준비'];
  const data2 =
  {
    
    labels: labels2,
    datasets: 
    [
      {
        label: '훈련 시간',
        data: DataList2,
        borderColor: 
        [
            '#FFC107',
            
        ],
        backgroundColor: 
        [
            '#FFC107',
            
        ],
        
      },
    ],
    
  };
  // </block:setup>
  const scale2 = 
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
  const config2 = 
  {
    plugins:[ChartDataLabels],
    type: 'bar',
    data: data2,
    options: 
    {
		indexAxis:'y',
       
		responsive: true,
		
		 scales: scale2,
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
            text : '시나리오별 평균 훈련시간(단위:분)',
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

  const barChart2 = new Chart( myChart2,  config2 );
  
  