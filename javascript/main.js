    let _WebSocket;
    let X_pos = [];
    let Y_pos = [];
    let num_X = [];
    let num_Y = [];
    let set;
    let send_interval;
    let line_number = 0;
    let pos_number = 0;
    let _isSend = 0;
    let nowradian = 0;
    function Drawing() {
        let button = document.getElementById("button");
        let canvas = document.getElementById('canvas');
        let context = canvas.getContext('2d');
        context.beginPath();
        context.fillStyle = "#e1e1e1";
        context.fillRect(0, 0, canvas.width, canvas.height);
         defcolor = '#'+Math.floor(Math.random()*16777215).toString(16);
        let defalpha = 1.0;
        let defsize = 10;
        let mouseX = "";
        let mouseY = "";
        button.addEventListener('click',function(e){
            if(!_isSend){
                send_interval = setInterval(send,500);  
            }else{
                $('output_div').append(send_compleat);
                clearInterval(send_interval);
            }
        });                  

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
                set = setInterval(savepostion,100);
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
        function savepostion(){
            X_pos.push(mouseX);
            Y_pos.push(mouseY);
            $('#output_div').append(mouseX+" , "+mouseY+"<br>");
            // 
        }
        function drawEnd(){
            clearInterval(set);
            num_X.push(X_pos);
            num_Y.push(Y_pos);
            mouseX = "";
            mosueY = "";
            defcolor = '#'+Math.floor(Math.random()*16777215).toString(16);
        }
    }

    function send(){
        //_WebSocket.send(num_X[line_number][pos_number]);
         //   console.log(line_number, num_X.length);
        if(line_number+2>num_X.length){
            clearInterval(send_interval);
            _WebSocket.send(190);
            return;
        }
            let radian = Math.atan2((num_Y[line_number][pos_number+1]-num_Y[line_number][pos_number]),(num_X[line_number][pos_number+1]-num_X[line_number][pos_number]));
        let angle = (radian-nowradian)/(Math.PI / 180);
        if(angle>180){
            angle = -(360 - angle);
        }
        if(angle < -180 ){
            angle = 360 + angle;
        }
        console.log(angle);
        _WebSocket.send(angle);
        nowradian = radian;
    //    _WebSocket.send(num_X[line_number][pos_number])+,+);
        pos_number++;
        if(pos_number+3>num_X[line_number].length){
            line_number++;
        }
    }

    function connect(){

        _WebSocket = new WebSocket($('#websocket_address').val());
        $('#WebSocket_State').text("Connecting");
        _WebSocket.onopen = function (){
            $('#WebSocket_State').html('connected');
        };
        _WebSocket.onmessage = function(e){
            $('#output_div').append(e.data+"<br>");
        };
        _WebSocket.onerror = function(){
            $('output_div').text("Error");
        };      
        _WebSocket.onclose = function(){
            $('output_div').text("Disconnected!!");
        };  
    }
    