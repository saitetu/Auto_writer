let canvas = document.getElementById('canvas');
if (screen.width < 860) {
    canvas.width = 700 * screen.width / 860;
    canvas.height = 400 * screen.width / 860;
}
if(canvas.getContext)   {
    Drawing();
}

function Drawing(){
    let context = canvas.getContext('2d');
    context.beginPath();
    context.fillStyle = "#e1e1e1"
    context.fillRect();
    let defcolor = "#ee9977";
    let defsize = 10;
    
}