function Drawing() {
    let canvas = document.getElementById('canvas');
    let context = canvas.getContext('2d');
    context.beginPath();
    context.fillStyle = "#e1e1e1";
    context.fillRect(0, 0, canvas.width, canvas.height);
    let defcolor = "#ee9977";
    let defalpha = 1.0;
    let defsize = 10;
    let mouseX = "";
    let mouseY = "";
    
    canvas.addEventListener('mousemove', onMove, false);
    canvas.addEventListener('mousedown', onClick, false);
    canvas.addEventListener('mouseup', drawEnd, false);
    canvas.addEventListener('mouseout', drawEnd , false);
    
    function onMove(e){
        if(e.buttons === 1 || e.witch === 1){
            var rect = e.target.getBoundingClientRect();
            var X = ~~(e.clientX - rect.left);
            var Y = ~~(e.clientY - rect.top);
            draw(X,Y);
        }
        
    }
    function onClick(e) {
        if (e.button === 0) {
            var rect = e.target.getBoundingClientRect();
            var X = ~~(e.clientX - rect.left);
            var Y = ~~(e.clientY - rect.top);
            draw(X, Y);
        }
    };
    function draw(X,Y){
        context.beginPath();
        context.globalAlpha = defalpha;
        if(mouseX === ""){
            context.moveTo(X,Y);
        }else{
            context.moveTo(mouseX,mouseY);
        }
        context.lineCap = "round";
        context.lineTo(X,Y);
        context.lineWidth = defsize*2;
        context.strokeStyle = defcolor;
        context.stroke();
        mouseX = X;
        mouseY = Y;
    }
    function drawEnd(){
        mouseX = "";
        mosueY = "";
        defcolor = '#'+Math.floor(Math.random()*16777215).toString(16);
    }
}