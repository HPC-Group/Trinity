    /****************************************************************
     * Copyright (C) Jürgen Grüninger 2016 and failed to copy by Andrey Krekhov
     * The content of this file may not be copied and/or distributed
     * without the expressed permission of the copyright owner.
     * 
     ****************************************************************/
    var TRI_Frontend = {

        connectFadeoutTimeout: undefined,
        slideCount: 0,
        dataSelected: false,
        datasetName: undefined,
        requirementPanelCount: 0,
        applicationTypes: [],
        deviceTypes: [],
        deviceDict: undefined,
        selectedApplicationName: undefined,
        selectedApplicationDescription: undefined,
        proposalRequestsCount: 0,
        proposalRequestsNeeded: 0,
        proposals: [],
        password: '',
        gridLeaper: false,
        canvas: undefined,
        img: undefined,
        ctx: undefined,

        init: function() {
            $("#apps").hide();
            $("#panel_requirements").hide();
            $("#div_connect_success").hide();
            $("#div_connect_failure").hide();
            $("#io_ip").val("134.91.11.156");
            $("#io_port").val("6678");
            $("#proc_ip").val("134.91.11.156");
            $("#x_res").val(800);
            $("#y_res").val(800);
            $("#proc_port").val("8679");

            $("#connect_ip").keyup(function(event) {
                if (event.keyCode == 13) {
                    $("#btn_connect").click();
                }
            });



            $("#connect_port").keyup(function(event) {
                if (event.keyCode == 13) {
                    $("#btn_connect").click();
                }
            });

            canvas = document.getElementById('myCanvas'),
                img = new Image(),
                ctx = canvas.getContext('2d'),
                $("#myCanvas").on("touchstart mousedown", function(e) {
                    TRI_Frontend.VisControlConnector.onDown(e)
                });


            canvas.style.width = '100%';
            canvas.style.height = '100%';
            // ...then set the internal size to match
            canvas.width = canvas.offsetWidth;
            canvas.height = canvas.offsetHeight;

            $("#myCanvas").on("touchmove mousemove", function(e) {
                TRI_Frontend.VisControlConnector.onMove(e)
            });
            $("#myCanvas").on("touchend mouseup", function(e) {
                TRI_Frontend.VisControlConnector.onUp(e)
            });

        },
        
        connectLeaper: function() {
            TRI_Frontend.gridLeaper = true;
            TRI_Frontend.connect();
        },

        connect: function() {
            canvas.width = $("#x_res").val();
            canvas.height = $("#y_res").val();
            
            if(!TRI_Frontend.dataSelected) {
                                $('#errorBody').text("please select a dataset first");
                $('#errorModal').modal('show');
            } else {
            TRI_Frontend.NodeConnector.initWS();
            TRI_Frontend.password = $("#connect_password").val();
            }
        },


        buildApplicationList: function() {
            //clear list
            $("#panel_requirements").hide();

            TRI_Frontend.slideCount = 0;
            var slides = $("#carousel_slides").children();
            slides[0].classList.add("active");
            var indicators = $("#carousel_indicators").children();
            indicators[0].classList.add("active");
            for (var i = 1; i < slides.length; i++) {
                slides[i].remove();
                indicators[i].remove();
            }

            var applicationsArray = ["bla", "blub"];
            console.log("Building applicatons list");
            
            var imgpath = "imagevis_body.png"; 
            TRI_Frontend.addApplicationSlide("CT", "UVF data", imgpath);
            
            imgpath = "bonsai.png";
            TRI_Frontend.addApplicationSlide("bonsai", "tiny bonsai tree", imgpath);
            
                imgpath = "imagevis_fractal_back.png";
            TRI_Frontend.addApplicationSlide("fractal", "analytical data", imgpath);
            
            imgpath = "meshkov.png";
            TRI_Frontend.addApplicationSlide("instability simulation", "Richtmeyer-Meshkov", imgpath);
            
            $("#apps").show();
            window.scrollTo(0, document.body.scrollHeight);
            
        },

        hideConnectMessage: function() {
            TRI_Frontend.connectFadeoutTimeout = setTimeout(function() {
                    $("#div_connect_success").fadeOut('slow');
                    $("#div_connect_failure").fadeOut('slow');
                },
                5000
            );

        },

        addApplicationSlide: function(name, description, image) {
            // WORKAROUND for Bootstrap Carousel
            // First slide needs "active" as additional class and needs to be present during webpage generation -> keep it in html and modify first item
            if (TRI_Frontend.slideCount == 0) {
                $("#carousel_slides").find("h1")[0].innerHTML = name;
                $("#carousel_slides").find("p")[0].innerHTML = description;
                $("#carousel_slides").find("img")[0].src = "img/mui/" + image;
                var linknode = $("#carousel_slides").find("a")[0];
                linknode.setAttribute("onclick", "TRI_Frontend.applicationSelected('" + name + "','" + description + "')");
            } else {
                var parameterString = "\"" + name + "\",\"" + description + "\"";
                //TODO: Insert correct image
                $("#carousel_slides").append("<div class='item'> \
                <img src='img/mui/" + image + "' alt='Chania'> \
                <div class='container'> \
                    <div class='carousel-caption'> \
                        <h1>" + name + "</h1> <p>" + description + "</p> <p> \
                        <a class='btn btn-lg btn-danger' role='button' onclick='TRI_Frontend.applicationSelected(" + parameterString + ")'>Select</a></p> \
                    </div> \
                </div> \
            </div>");
                $("#carousel_indicators").append("<li data-target='#applicationCarousel' data-slide-to='" + TRI_Frontend.slideCount + "'></li>");
            }

            TRI_Frontend.slideCount++;
        },
        
        applicationSelected : function (name, description)
	   {
           
        TRI_Frontend.dataSelected = true;
        $("#render_panel").show();
        console.log("selected dataset " + name);
           
           if(name == "CT") {
               TRI_Frontend.datasetName = "UVFData@./WholeBody-SCANLINE-132-lz4.uvf";
           }
           
            if(name == "bonsai") {
               TRI_Frontend.datasetName = "UVFData@./Bonsai-SCANLINE-132-lz4.uvf";
           }
           
            if(name == "fractal") {
               TRI_Frontend.datasetName = "UVFData@./Mandelbulb1k-SCANLINE-132-lz4.uvf";
           }
           
                      
            if(name == "instability simulation") {
               TRI_Frontend.datasetName = "UVFData@./RichtmyerMeshkov-SCANLINE-132-lz4.uvf";
           }
        //empty requirements, reset requirements row counter and disable start ui button
           /*
        $("#panel_requirements").hide();
        MUI_MasterUI.requirementPanelCount = 0;
        $("#container_requirements").empty();
        $('#btnStartUI').prop('disabled', true);
        MUI_MasterUI.proposalRequestsCount = 0;
        MUI_MasterUI.proposalRequestsNeeded = 0;
        MUI_MasterUI.proposals = [];
        
        //send Requirements request
        MUI_MasterUI.selectedApplicationName = name;
        MUI_MasterUI.selectedApplicationDescription = description;
        
        if (MUI_MasterUI.applicationTypes)
        {
            for (var i = 0; i<MUI_MasterUI.applicationTypes.length; i++)
            {
                if (MUI_MasterUI.applicationTypes[i].id == MUI_MasterUI.selectedApplicationName)
                {
                    console.log("Building requirements list");
                    //query proposals
                    MUI_MasterUI.queryMuiProposals(MUI_MasterUI.applicationTypes[i].requirements, MUI_MasterUI.selectedApplicationDescription);
                    //MUI_MasterUI.buildRequirementList(MUI_MasterUI.applicationTypes[i].requirements);
                    break;
                }
            }
        }*/
	   },


        hidePanel: function() {
            TRI_Frontend.connectFadeoutTimeout = setTimeout(function() {

                    $("#connect_panel").fadeOut('fast');
                },
                500
            );

        },


    }


    // NodeConnector handles the WS connection and defines the behaviour
    TRI_Frontend.NodeConnector = (function() {

        var messageDict = undefined;
        var parameterDict = undefined;
        var websocket = undefined;


        return {
            wsUri: "",

            initWS: function() {
                console.log("Initializing Websocket Connection");
                //messageDict = new TRI_Frontend.Dictionary();
                //parameterDict = new TRI_Frontend.Dictionary();
                TRI_Frontend.NodeConnector.wsUri = "ws://" + $("#proc_ip")[0].value + ":" + $("#proc_port")[0].value + "/";

                websocket = new WebSocket(TRI_Frontend.NodeConnector.wsUri);
                websocket.onopen = function(evt) {
                    TRI_Frontend.NodeConnector.onOpen(evt)
                };
                websocket.onclose = function(evt) {
                    TRI_Frontend.NodeConnector.onClose(evt)
                };
                websocket.onmessage = function(evt) {
                    TRI_Frontend.NodeConnector.onMessage(evt)
                };
                websocket.onerror = function(evt) {
                    TRI_Frontend.NodeConnector.onError(evt)
                };

            },

            onOpen: function(evt) {
                console.log("Websocket CONNECTED to " + TRI_Frontend.NodeConnector.wsUri);
                $("#div_connect_success").fadeIn('slow', TRI_Frontend.hidePanel);
                console.log("sending init renderer request");
                TRI_Frontend.NodeConnector.sendRequest("InitRenderer");
                console.log("done sending init renderer request");
            },

            onClose: function(evt) {
                console.log("Websocket DISCONNECTED");
            },

            onMessage: function(evt) {
                //console.log("message");
                //console.log(evt.data);

                var messageResult = {};

                try {
                    messageResult = JSON.parse(evt.data);
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

            onError: function(evt) {
                console.log("Websocket Error: " + evt.data);
                $("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            },

            doSend: function(message) {
                console.log("Sent from nodeconnector: " + message);
                websocket.send(message);
            },

            sendRequest: function(requestType, parameter) {
                var requestTypes = ["InitRenderer"];
                if (requestTypes.indexOf(requestType) != -1) {
                    // look like type: InitRenderer; rid: 1; sid: 0; params: { protocol: tcp.prefixed; rendertype: SimpleRenderer; fileid: FractalData@3; ioendpoint: tcp.prefixed:127.0.0.1:6678; streamingparams: { xres: 800; yres: 600 } }
                    console.log("Sending: " + requestType);
                    var ioString = "" + $("#io_ip")[0].value + ":" + $("#io_port")[0].value;
                    console.log("io endpoint: " + ioString);
                    
                    if(TRI_Frontend.gridLeaper) {
                    TRI_Frontend.NodeConnector.doSend('{"type": "InitRenderer",  "req": { "rid": 1, "sid": 0, "params": { "protocol": "tcp.ws", "rendertype": "GridLeapingRenderer", "fileid": "'+ TRI_Frontend.datasetName + '", "ioendpoint": "tcp.prefixed:'  + ioString + '", "streamingparams": { "xres": ' + $("#x_res").val() + ', "yres": ' + $("#y_res").val() + ' } } } }');
                    }
                    else {
                    TRI_Frontend.NodeConnector.doSend('{"type": "InitRenderer",  "req": { "rid": 1, "sid": 0, "params": { "protocol": "tcp.ws", "rendertype": "SimpleRenderer", "fileid": "'+ TRI_Frontend.datasetName + '", "ioendpoint": "tcp.prefixed:'  + ioString + '", "streamingparams": { "xres": ' + $("#x_res").val() + ', "yres": ' + $("#y_res").val() + ' } } } }');
                    }
                        
                        TRI_Frontend.gridLeaper = false;

                }

            }
        };

    })();



    // NodeConnector handles the WS connection and defines the behaviour
    TRI_Frontend.VisControlConnector = (function() {

        var websocketControl = undefined;
        var port = undefined;


        var lastX = undefined;
        var lastY = undefined;
        var isRotating = false;
        var deltaX = 0.0;
        var deltaY = 0.0;
        var stoppedSending = false;
        var rotMode = true;
        var zoomCounter = 0;
        var zoomOutNeeded = true;
        var currentTF = 0;
        var tfCount = 0;




        return {

            controlConnectionUri: "",

            onMove: function(e) {

                if (isRotating)  {
                    //calculate deltas. mouse movement to the right and up is positive
                    if (e.type == "touchmove") {
                        deltaX = e.originalEvent.touches[0].pageX - lastX;
                        deltaY = lastY - e.originalEvent.touches[0].pageY;

                        lastX = e.originalEvent.touches[0].pageX;
                        lastY = e.originalEvent.touches[0].pageY;
                    } else {
                        deltaX += e.clientX - lastX;
                        deltaY += lastY - e.clientY;


                        lastX = e.clientX;
                        lastY = e.clientY;
                    }
                    

                    //buildSendMessage(deltaX,deltaY,0);
                    if (stoppedSending) {
                        stoppedSending = false;
                        TRI_Frontend.VisControlConnector.sendRotation();

                    }
                }

            },

            switchMode: function() {
                //rotMode = !rotMode;
                if (rotMode) {
                    rotMode = false;
                    $("#rotate").text('switch to rotation mode');
                    //$("#rotate").unfo;
                    document.activeElement.blur();

                    console.log("setting text to rotate");
                } else {
                    rotMode = true;
                    $("#rotate").text('switch to move mode');
                    console.log("setting text to move");
                    document.activeElement.blur();
                }
            },
            
            sendingWasStopped: function() {
                stoppedSending = true;
    

            },
            


            prevTF: function() {
                document.activeElement.blur();
                console.log("previous tf");
                currentTF--;
                TRI_Frontend.VisControlConnector.doSend('{"type": "GetDefault1DTransferFunctionCountProc", "req": { "rid": 1, "sid": 3, "params": {} } }');
            },

            nextTF: function() {
                document.activeElement.blur();
                console.log("next tf");
                currentTF++;
                TRI_Frontend.VisControlConnector.doSend('{"type": "GetDefault1DTransferFunctionCountProc", "req": { "rid": 1, "sid": 3, "params": {} } }');
                
                

                

                
                
                

            },
            
            
            sendTFRequest: function() {
                if(currentTF  == tfCount) { // jump to first tf
                    currentTF = 0;
                }
                if(currentTF == -1) {  // jump to last tf
                    currentTF = tfCount - 1;
                }
                
                console.log("requesting tf with index " + currentTF);
                console.log("total amount: " + tfCount);
                TRI_Frontend.VisControlConnector.doSend('{"type": "GetDefault1DTransferFunctionProc", "req": { "rid": 1, "sid": 3, "params": {"index":' + currentTF + '} } }');
            },
            
            zoomIn: function() {
                zoomOutNeeded = false;
                document.activeElement.blur();
                zoomCounter = 20;
                TRI_Frontend.VisControlConnector.sendZoomIn();
    

            },
            
            needsZoom: function() {
                if(zoomCounter != 0)
                    return true;
                else return false;
            },
            
            needsZoomOut: function() {
                return zoomOutNeeded;
            },
            
            zoomOut: function() {
                zoomOutNeeded = true;
                document.activeElement.blur();
                zoomCounter = 20;
                TRI_Frontend.VisControlConnector.sendZoomOut();
                
                    
                
            },
            
            sendZoomIn: function() {
                zoomCounter--;
                if(zoomCounter > 0)
                    TRI_Frontend.VisControlConnector.doSend('{"req":{"params":{"zoom": 1.007},"rid":108,"sid":1},"type":"ZoomCamera"}');
            },
            
            sendZoomOut: function() {
                zoomCounter--;
                if(zoomCounter > 0)
                    TRI_Frontend.VisControlConnector.doSend('{"req":{"params":{"zoom": 0.993},"rid":108,"sid":1},"type":"ZoomCamera"}');
            },



            onDown: function(e) {
                e.preventDefault();
                //console.log(e);

                isRotating = true;
                if (e.type == "touchstart") {
                    lastX = e.originalEvent.touches[0].pageX;
                    lastY = e.originalEvent.touches[0].pageY;
                } else {
                    lastX = e.clientX;
                    lastY = e.clientY;
                }

            },

            onUp: function(e) {

                isRotating = false;
            },
            
           needsRotation: function(e) {

                if (Math.abs(deltaX) < 0.0001 && Math.abs(deltaY) < 0.0001)
                    return false;
               else return true;
            },




            sendRotation() {

                if (!TRI_Frontend.VisControlConnector.needsRotation()) {
                    stoppedSending = true;
                } else {

                    if (rotMode) {
                        TRI_Frontend.VisControlConnector.doSend('{"req":{"params":{"rotation":{"x": ' + deltaY * (-0.01) + ',"y":' + deltaX * 0.01 + ',"z":0}},"rid":108,"sid":1},"type":"RotateScene"}');
                    } else {
                        TRI_Frontend.VisControlConnector.doSend('{"req":{"params":{"direction":{"x": ' + deltaX * (0.001) + ',"y":' + deltaY * 0.001 + ',"z":0}},"rid":108,"sid":1},"type":"MoveCamera"}');
                    }
                    deltaX = 0.0;
                    deltaY = 0.0;
                }

            },


            initWS: function(controlport) {
                port = controlport;
                console.log("Initializing Vis Control Connection to port " + port);
                //messageDict = new TRI_Frontend.Dictionary();
                //parameterDict = new TRI_Frontend.Dictionary();
                TRI_Frontend.VisControlConnector.controlConnectionUri = "ws://" + $("#proc_ip")[0].value + ":" + port + "/";

                websocketControl = new WebSocket(TRI_Frontend.VisControlConnector.controlConnectionUri);
                websocketControl.onopen = function(evt) {
                    TRI_Frontend.VisControlConnector.onOpen(evt)
                };
                websocketControl.onclose = function(evt) {
                    TRI_Frontend.VisControlConnector.onClose(evt)
                };
                websocketControl.onmessage = function(evt) {
                    TRI_Frontend.VisControlConnector.onMessage(evt)
                };
                websocketControl.onerror = function(evt) {
                    TRI_Frontend.VisControlConnector.onError(evt)
                };

            },

            onOpen: function(evt) {
                console.log("Websocket CONNECTED to " + TRI_Frontend.VisControlConnector.controlConnectionUri);
                $("#div_connect_success").fadeIn('slow', TRI_Frontend.hideConnectMessage);
                TRI_Frontend.VisControlConnector.doSend('{"type": "StartRendering", "req": { "rid": 1, "sid": 3, "params": {} } }');



                $("#div_connect_success").fadeIn('slow', TRI_Frontend.hideConnectMessage);

                console.log("done sending init context request");
            },

            onClose: function(evt) {
                console.log("Websocket DISCONNECTED");
            },

            onMessage: function(evt) {
                //console.log("message");
                //console.log(evt.data);

                var messageResult = {};

                try {
                    messageResult = JSON.parse(evt.data);
                    console.log(messageResult);
                    console.log("incoming: " + messageResult.rep.params.result);
                } catch (error) {
                    console.log("ERROR: Error while parsing Message response to JSON")
                }
                console.log("got sth in control channel:");
                
                if(messageResult.type == "GetDefault1DTransferFunctionCountProc") {
                    tfCount = messageResult.rep.params.result;
                     TRI_Frontend.VisControlConnector.sendTFRequest();
                }
                
                if(messageResult.type == "GetDefault1DTransferFunctionProc") {
                    TRI_Frontend.VisControlConnector.doSend('{"type": "Set1DTransferFunction", "req": { "rid": 1, "sid": 3, "params": {"tf": '+ JSON.stringify(messageResult.rep.params.result) + '} } }');
                }
            },

            onError: function(evt) {
                console.log("Websocket Error: " + evt.data);
                $("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            },

            doSend: function(message) {
                //console.log("Sent: " + message);
                websocketControl.send(message);
            }
        };
    })();



    // NodeConnector handles the WS connection and defines the behaviour
    TRI_Frontend.StreamConnector = (function() {

        var websocketStream = undefined;
        var port = undefined;
        var rot = 0.0;
        var url = undefined;



        return {
            visConnectionUri: "",

            initWS: function(visport) {
                port = visport;

                //messageDict = new TRI_Frontend.Dictionary();
                //parameterDict = new TRI_Frontend.Dictionary();
                TRI_Frontend.StreamConnector.visConnectionUri = "ws://" + $("#proc_ip")[0].value + ":" + port + "/";
                console.log("Initializing Stream Connection to " + TRI_Frontend.StreamConnector.visConnectionUri);
                websocketStream = new WebSocket(TRI_Frontend.StreamConnector.visConnectionUri);
                websocketStream.binaryType = "blob";
                websocketStream.onopen = function(evt) {
                    TRI_Frontend.StreamConnector.onOpen(evt)
                };
                websocketStream.onclose = function(evt) {
                    TRI_Frontend.StreamConnector.onClose(evt)
                };
                websocketStream.onmessage = function(evt) {
                    TRI_Frontend.StreamConnector.onMessage(evt)
                };
                websocketStream.ondata = function(src, start, end) {
                    TRI_Frontend.StreamConnector.onData(src, start, end)
                };
                websocketStream.onerror = function(evt) {
                    TRI_Frontend.StreamConnector.onError(evt)
                };
                document.getElementById("vis_panel").style["display"] = "block";


            },

            onOpen: function(evt) {
                console.log("Websocket CONNECTED to " + TRI_Frontend.StreamConnector.visConnectionUri);
                $("#div_connect_success").fadeIn('slow', TRI_Frontend.hideConnectMessage);
                console.log("not doing enything with vis stream");
            },

            onClose: function(evt) {
                console.log("Websocket DISCONNECTED");
            },

            onMessage: function(evt) {
                //console.log("gim in!");
                //console.log(evt.data);
                if(url != undefined)
                    URL.revokeObjectURL(url);

                img.onload = function() {
                        //console.log("drawing...");
                        ctx.drawImage(img, 0, 0);
                    }
                    //console.log("replacing image source");
                
                url = URL.createObjectURL(evt.data);
                img.src = url;
                
                if(TRI_Frontend.VisControlConnector.needsZoom()) {
                    
                    if(TRI_Frontend.VisControlConnector.needsZoomOut()) {
                        TRI_Frontend.VisControlConnector.sendZoomOut();
                    } else {
                        TRI_Frontend.VisControlConnector.sendZoomIn();
                    }
                    
                } else { if(TRI_Frontend.VisControlConnector.needsRotation()) {
                
                    //console.log("need rot");
                    TRI_Frontend.VisControlConnector.sendRotation();
                    
                } else {
                    //console.log("refining");
                    TRI_Frontend.VisControlConnector.doSend('{"type": "ProceedRendering", "req": { "rid": 1, "sid": 3, "params": {} } }');
                    TRI_Frontend.VisControlConnector.sendingWasStopped();
                    
                } 
                }
                
                
                
                //URL.revokeObjectURL(url);
                //rot += 0.01;

            },

            onError: function(evt) {
                console.log("Websocket Error: " + evt.data);
                $("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            },

            doSend: function(message) {
                //console.log("Sent: " + message);
                websocketStream.send(message);
            }
        };
    })();