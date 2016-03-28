/****************************************************************
* Copyright (C) Jürgen Grüninger 2016 and failed to copy by Andrey Krekhov
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
* 
****************************************************************/


var TRI_Frontend = {

	connectFadeoutTimeout : undefined,
	slideCount : 0,
	requirementPanelCount : 0,
    applicationTypes : [],
    deviceTypes : [],
    deviceDict : undefined,
    selectedApplicationName : undefined,
    selectedApplicationDescription : undefined,
    proposalRequestsCount : 0,
    proposalRequestsNeeded : 0,
    proposals : [],
    password : '',

	init : function ()
	{
		$("#apps").hide();
		$("#panel_requirements").hide();
		$("#div_connect_success").hide();
		$("#div_connect_failure").hide();
        $("#io_ip").val("127.0.0.1");
        $("#io_port").val("6679");
        $("#proc_ip").val("127.0.0.1");
        $("#proc_port").val("8679");

		$("#connect_ip").keyup(function(event){
			if(event.keyCode == 13){
				$("#btn_connect").click();
			}
		});
		$("#connect_port").keyup(function(event){
			if(event.keyCode == 13){
				$("#btn_connect").click();
			}
		});


		// registering mousemove while mousedown
		$("#myCanvas").mousedown(function () {
    		$(this).mousemove(function (evt) {
        	TRI_Frontend.VisControlConnector.onMove(evt.clientX, evt.clientY);
    		});

		}).mouseup(function () {
    		$(this).unbind('mousemove'); 
    	}).mouseout(function () {
    
    	$(this).unbind('mousemove');
		});

	},
	
	connect : function ()
	{
		TRI_Frontend.NodeConnector.initWS();
        TRI_Frontend.password = $("#connect_password").val()
	},
	
	hideConnectMessage : function () 
	{
		TRI_Frontend.connectFadeoutTimeout = setTimeout( function () 
			{
				 $("#div_connect_success").fadeOut('slow');
				 $("#div_connect_failure").fadeOut('slow');
			},
			5000
			);
		
	},
	
}


// NodeConnector handles the WS connection and defines the behaviour
TRI_Frontend.NodeConnector = (function (){
	
	var messageDict = undefined;
    var parameterDict = undefined;
	var websocket = undefined;
	
	function getRandomInt(min, max)
	{
		return Math.floor(Math.random() * (max - min)) + min;
	}
	
	function getRandomId()
	{
		var randInt = undefined;

		if (messageDict !== undefined)
		{
			while (true)
			{
				randInt = getRandomInt(0, 9999999999999);
				if ( messageDict.getValue(randInt) === undefined) break;
			}
		}
		return randInt;
	}

	return {
		wsUri : "",

		initWS : function() 
		{ 
            console.log("Initializing Websocket Connection");
			//messageDict = new TRI_Frontend.Dictionary();
            //parameterDict = new TRI_Frontend.Dictionary();
			TRI_Frontend.NodeConnector.wsUri = "ws://"+$("#proc_ip")[0].value+":"+$("#proc_port")[0].value+"/";

			websocket = new WebSocket(TRI_Frontend.NodeConnector.wsUri);
			websocket.onopen = function(evt) { TRI_Frontend.NodeConnector.onOpen(evt) };
			websocket.onclose = function(evt) { TRI_Frontend.NodeConnector.onClose(evt) };
			websocket.onmessage = function(evt) { TRI_Frontend.NodeConnector.onMessage(evt) };
			websocket.onerror = function(evt) { TRI_Frontend.NodeConnector.onError(evt) };
		
		},

		onOpen : function(evt) 
		{
			console.log("Websocket CONNECTED to " + TRI_Frontend.NodeConnector.wsUri);
			$("#div_connect_success").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            console.log("sending init renderer request");
            TRI_Frontend.NodeConnector.sendRequest("InitRenderer");
            console.log("done sending init renderer request");
		},

		onClose : function(evt) 
		{
			console.log("Websocket DISCONNECTED");
		},

		onMessage : function(evt)
		{
			//console.log("message");
			//console.log(evt.data);
            
			var messageResult = {};

            try {
                messageResult = JSON.parse(evt.data);
                console.log(messageResult);
            } catch (error) {
                console.log("ERROR: Error while parsing Message response to JSON")
            }
            console.log("result:");
            console.log(messageResult);
            console.log(messageResult.rep.params);


            if (messageResult.rep.params !== undefined) {
                TRI_Frontend.StreamConnector.initWS(messageResult.rep.params.visport);

                TRI_Frontend.VisControlConnector.initWS(messageResult.rep.params.controlport);
            }
		},

		onError : function(evt)
		{
			console.log("Websocket Error: " + evt.data);
			$("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
		},

		doSend : function(message)
		{
			//console.log("Sent: " + message);
			websocket.send(message);
		},
	
		sendRequest : function(requestType, parameter) 
		{
			var requestTypes = ["InitRenderer"];
			if (requestTypes.indexOf(requestType) != -1 )
            {
                        // look like type: InitRenderer; rid: 1; sid: 0; params: { protocol: tcp.prefixed; rendertype: SimpleRenderer; fileid: FractalData@3; ioendpoint: tcp.prefixed:127.0.0.1:6678; streamingparams: { xres: 800; yres: 600 } }
                        console.log("Sending: " + requestType);
                        TRI_Frontend.NodeConnector.doSend('{"type": "InitRenderer",  "req": { "rid": 1, "sid": 0, "params": { "protocol": "tcp.ws", "rendertype": "SimpleRenderer", "fileid": "UVFData@head512.uvf", "ioendpoint": "tcp.prefixed:127.0.0.1:6678", "streamingparams": { "xres": 2000, "yres": 2000 } } } }');
            
			}

		}
	};
	
})();



// NodeConnector handles the WS connection and defines the behaviour
TRI_Frontend.VisControlConnector = (function () {
    
    var websocketControl = undefined;
    var port = undefined;
    var x = 0.0;
    var y = 0.0;
    var deltax = 0.0;
    var deltay = 0.0;
    var rot = 0.0;
    
    

    return {

        controlConnectionUri: "", 

        onMove: function (xnew, ynew) {

	        	deltax = xnew - x;

	        	if(deltax > 2.0 || deltax < -2.0) {
	        		x = xnew;
	        	} else {
	        	//console.log("x: " + deltax);
	        	x = xnew;
	        	console.log("x: " + deltax);
	        	rot = rot + deltax * 0.001;
	        }
	        	
        },

        sendRotation() {
        	TRI_Frontend.VisControlConnector.doSend('{"type": "SetIsoValue", "req": { "rid": 1, "sid": 3, "params": { "isovalue": '+ rot + ' } } }');
        },


        initWS : function(controlport) 
        { 
            port = controlport;
            console.log("Initializing Vis Control Connection to port " + port);
            //messageDict = new TRI_Frontend.Dictionary();
            //parameterDict = new TRI_Frontend.Dictionary();
            TRI_Frontend.VisControlConnector.controlConnectionUri = "ws://" + $("#proc_ip")[0].value+":" + port + "/";

            websocketControl = new WebSocket(TRI_Frontend.VisControlConnector.controlConnectionUri);
            websocketControl.onopen = function(evt) { TRI_Frontend.VisControlConnector.onOpen(evt) };
            websocketControl.onclose = function(evt) { TRI_Frontend.VisControlConnector.onClose(evt) };
            websocketControl.onmessage = function(evt) { TRI_Frontend.VisControlConnector.onMessage(evt) };
            websocketControl.onerror = function(evt) { TRI_Frontend.VisControlConnector.onError(evt) };
        
        },

        onOpen : function(evt) 
        {
            console.log("Websocket CONNECTED to " + TRI_Frontend.VisControlConnector.controlConnectionUri);
            $("#div_connect_success").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            TRI_Frontend.VisControlConnector.doSend('{"type": "InitContext", "req": { "rid": 1, "sid": 3, "params": { "dummy": 1} } }');
            TRI_Frontend.VisControlConnector.doSend('{"type": "SetIsoValue", "req": { "rid": 1, "sid": 3, "params": { "isovalue": 0 } } }');

            console.log("done sending init context request");
        },

        onClose : function(evt) 
        {
            console.log("Websocket DISCONNECTED");
        },

        onMessage : function(evt)
        {
            //console.log("message");
            //console.log(evt.data);
            
            var messageResult = {};

            try {
                messageResult = JSON.parse(evt.data);
                console.log(messageResult);
            } catch (error) {
                console.log("ERROR: Error while parsing Message response to JSON")
            }
            console.log("got sth in control channel:");
        },

        onError : function(evt)
        {
            console.log("Websocket Error: " + evt.data);
            $("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
        },

        doSend : function(message)
        {
            console.log("Sent: " + message);
            websocketControl.send(message);
        }
    };
})();



// NodeConnector handles the WS connection and defines the behaviour
TRI_Frontend.StreamConnector = (function (){

    var websocketStream = undefined;
    var port = undefined;
    var rot = 0.0;
    var canvas = document.getElementById('myCanvas');
    var img = new Image();
    var ctx = canvas.getContext('2d');
    


    return {
        visConnectionUri: "",

        initWS : function(visport) 
        { 
            port = visport;
            
            //messageDict = new TRI_Frontend.Dictionary();
            //parameterDict = new TRI_Frontend.Dictionary();
            TRI_Frontend.StreamConnector.visConnectionUri = "ws://"+$("#proc_ip")[0].value+":"+ port +"/";
            console.log("Initializing Stream Connection to " + TRI_Frontend.StreamConnector.visConnectionUri);
            websocketStream = new WebSocket(TRI_Frontend.StreamConnector.visConnectionUri);
            websocketStream.binaryType = "blob";
            websocketStream.onopen = function(evt) { TRI_Frontend.StreamConnector.onOpen(evt) };
            websocketStream.onclose = function(evt) { TRI_Frontend.StreamConnector.onClose(evt) };
            websocketStream.onmessage = function(evt) { TRI_Frontend.StreamConnector.onMessage(evt) };
            websocketStream.ondata = function(src,start,end) { TRI_Frontend.StreamConnector.onData(src,start,end) };
            websocketStream.onerror = function(evt) { TRI_Frontend.StreamConnector.onError(evt) };
        	document.getElementById("myCanvas").style["display"] = "block";
        },

        onOpen : function(evt) 
        {
            console.log("Websocket CONNECTED to " + TRI_Frontend.StreamConnector.visConnectionUri);
            $("#div_connect_success").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            console.log("not doing enything with vis stream");
        },

        onClose : function(evt) 
        {
            console.log("Websocket DISCONNECTED");
        },

        onMessage : function(evt)
        {
            //console.log("gim in!");
            //console.log(evt.data);

            img.onload = function () {
                //console.log("drawing...");
                ctx.drawImage(img,0,0);
            }
            //console.log("replacing image source");

            img.src = URL.createObjectURL(evt.data);
            TRI_Frontend.VisControlConnector.sendRotation();
            //rot += 0.01;
            
        },

        onError : function(evt)
        {
            console.log("Websocket Error: " + evt.data);
            $("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
        },

        doSend : function(message)
        {
            //console.log("Sent: " + message);
            websocketStream.send(message);
        }
    };
})();