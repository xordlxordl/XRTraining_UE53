

var jsonObject = JSON.stringify(jsonData);

var jData = JSON.parse(jsonObject);

var labelList = new Array();



const myChart4 = document.getElementById('mychart4').getContext('2d');
// <block:setup:1>
const DATA_COUNT4 = 12;
const labels4 = ['1월','2월','3월','4월','5월','6월','7월','8월','9월','10월','11월','12월'];//Utils.months({count: 12});

var iMaxCount = 0;
var iCurCount = 0;

for(var i = 0; i<jData.length; i++) 
{
	var d = jData[i];
	labelList.push( d.ncount );
	iCurCount = d.ncount;
	
	if( iCurCount > iMaxCount )
		iMaxCount = iCurCount

}
console.log(jsonObject);

const data4 = 
{
  labels: labels4,
  datasets: 
  [
    {
      label: '월별 등록수',
      data: labelList,
      borderColor:'#DC3545',
      fill: false,
      pointStyle: 'circle',
      pointRadius: 5,
      pointHoverRadius: 10,
      tension: 0,
      borderWidth: 1,
      backgroundColor:'#DC3545',
      
    },
  ]
};
// </block:setup>

// <block:config:0>
const config4 = 
{
  type: 'line',
  data: data4,
  options: 
  {
    responsive: true,
    plugins: 
    {
		 legend: 
		 {
			labels: 
			{
				color:'rgba(255,255,255,0.9)',
			},
		},
		title: 
		{
			display: true,
			text: '월별 훈련생 등록 수 (단위:명)',
			color: '#fff',
			align:'center',
			font :
			{
			    size:26,
			    weight:'lighter'
			}
		},
		
    },
    interaction: 
    {
      intersect: false,
    },
    scales: 
    {
		x: 
		  {
		    display: true,
		    title: 
		    {
		      display: true
		    },
		    grid:
		  	{
				color:'rgba(255,255,255,0)',
		  	},
		  	ticks:
		  	{
				color:'rgba(255,255,255,0.9)',
				weight:'lighter'
		  	}
			
		  },
		y: 
		  {
		    display: true,
		    title: 
		    	{
		      		display: true,
		      		text: '등록수(단위:명)',
		      		color:'rgba(255,255,255,0.9)',
		    	},
		    suggestedMin: 0,
		    suggestedMax: iMaxCount + 1,
		    grid:
		  		{
					color:'rgba(255,255,255,0.5)',
		  		},
		  },
    }
  },
};
// </block:config>
const barChart4 = new Chart( myChart4,  config4 );
