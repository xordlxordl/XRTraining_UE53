
const canvas = document.getElementById('mycanvas');
const canvas2 = document.getElementById('mycanvas2');

const ctx = canvas.getContext('2d');

const ctx2 = canvas2.getContext('2d');
function DrawGraph()
{
    ctx2.beginPath();
    let y = 5, w = 100, h = 100;
    ctx2.fillRect( xx, y, w, h);
    ctx2.fillStyle = 'red';
    ctx2.fill();
    ctx2.stroke();
}


function DrawRect()
{
    let x = 10, y = 50, w = 200, h = 100;
    ctx.fillStyle = "red"
    ctx.fill();
    ctx.fillRect( x, y, w, h );
    ctx.stroke();
}
DrawRect();

function DrawCircle()
{
    ctx.beginPath();
    let x = 300, y = 300, r = 30, s = 0, e = Math.PI * 2;
    ctx.arc( x, y, r, s, e );
    ctx.fillStyle = 'blue';
    ctx.fill();
    ctx.stroke();
}
DrawCircle();

function DrawTriangle()
{
    ctx.beginPath();
    ctx.moveTo( 200, 200 );
    ctx.lineTo( 200, 250 );
    ctx.lineTo( 250, 250 );
    ctx.fillStyle = 'red';
    ctx.fill();
    ctx.stroke();
}
DrawTriangle();
let xx = 0;
function MoveBall()
{
    ctx.clearRect( 0, 0, 1000, 1000 );
    ctx.beginPath();
    let  y = 300, r = 30, s = 0, e = Math.PI * 2;
    ctx.arc( xx, y, r, s, e );
    ctx.fillStyle = 'blue';
    ctx.fill();
    ctx.stroke();
    DrawGraph();
    xx += 1;
    if( xx >= 970 )
    {
        return;
    }
    requestAnimationFrame( MoveBall );
}
MoveBall();

