    /****************************************************************
     * Copyright (C) Jürgen Grüninger 2016 and failed to copy by Andrey Krekhov
     * The content of this file may not be copied and/or distributed
     * without the expressed permission of the copyright owner.
     * 
     ****************************************************************/
    var TRI_Frontend = {

        connectFadeoutTimeout: undefined,
        slideCount: 0,
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
        canvas: undefined,
        img: undefined,
        ctx: undefined,

        init: function() {
            $("#apps").hide();
            $("#panel_requirements").hide();
            $("#div_connect_success").hide();
            $("#div_connect_failure").hide();
            $("#io_ip").val("127.0.0.1");
            $("#io_port").val("6679");
            $("#proc_ip").val("127.0.0.1");
            $("#x_res").val(document.body.clientWidth);
            $("#y_res").val(window.innerHeight);
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

                // registering mousemove while mousedown
                $("#myCanvas").mousedown(function() {
                    $(this).mousemove(function(evt) {
                        TRI_Frontend.VisControlConnector.onMove(evt.clientX, evt.clientY);
                    });

                }).mouseup(function() {
                    $(this).unbind('mousemove');
                }).mouseout(function() {

                    $(this).unbind('mousemove');
                });
            
            canvas.addEventListener("touchmove", function(e) {
                e.preventDefault();
                TRI_Frontend.VisControlConnector.onMove(e.changedTouches[0].pageX, e.changedTouches[0].pageY);
                
        //alert(e.changedTouches[0].pageX) // alert pageX coordinate of touch point
            }, false);

            //TRI_Frontend.buildApplicationList();

        },

        connect: function() {
            canvas.width = $("#x_res").val();
            canvas.height = $("#y_res").val();
            TRI_Frontend.NodeConnector.initWS();
            TRI_Frontend.password = $("#connect_password").val()
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
            if (applicationsArray.length > 0) {
                for (var i = 0; i < applicationsArray.length; i++) {

                    var imgpath = undefined;
                    if (i % 2) {
                        imgpath = "imagevis_fractal.png";
                    } else {
                        imgpath = "imagevis_fractal_back.png";
                    }
                    TRI_Frontend.addApplicationSlide("peter", "coole app", imgpath);
                }
                $("#apps").show();
                window.scrollTo(0, document.body.scrollHeight);
            }
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
                linknode.setAttribute("onclick", "MUI_MasterUI.applicationSelected('" + name + "','" + description + "')");
            } else {
                var parameterString = "\"" + name + "\",\"" + description + "\"";
                //TODO: Insert correct image
                $("#carousel_slides").append("<div class='item'> \
                <img src='img/mui/" + image + "' alt='Chania'> \
                <div class='container'> \
                    <div class='carousel-caption'> \
                        <h1>" + name + "</h1> <p>" + description + "</p> <p> \
                        <a class='btn btn-lg btn-primary' role='button' onclick='MUI_MasterUI.applicationSelected(" + parameterString + ")'>Select</a></p> \
                    </div> \
                </div> \
            </div>");
                $("#carousel_indicators").append("<li data-target='#applicationCarousel' data-slide-to='" + TRI_Frontend.slideCount + "'></li>");
            }

            TRI_Frontend.slideCount++;
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

            onError: function(evt) {
                console.log("Websocket Error: " + evt.data);
                $("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            },

            doSend: function(message) {
                //console.log("Sent: " + message);
                websocket.send(message);
            },

            sendRequest: function(requestType, parameter) {
                var requestTypes = ["InitRenderer"];
                if (requestTypes.indexOf(requestType) != -1) {
                    // look like type: InitRenderer; rid: 1; sid: 0; params: { protocol: tcp.prefixed; rendertype: SimpleRenderer; fileid: FractalData@3; ioendpoint: tcp.prefixed:127.0.0.1:6678; streamingparams: { xres: 800; yres: 600 } }
                    console.log("Sending: " + requestType);
                    TRI_Frontend.NodeConnector.doSend('{"type": "InitRenderer",  "req": { "rid": 1, "sid": 0, "params": { "protocol": "tcp.ws", "rendertype": "SimpleRenderer", "fileid": "FractalData@3a", "ioendpoint": "tcp.prefixed:127.0.0.1:6678", "streamingparams": { "xres": ' + $("#x_res").val() + ', "yres": ' + $("#y_res").val() + ' } } } }');

                }

            }
        };

    })();



    // NodeConnector handles the WS connection and defines the behaviour
    TRI_Frontend.VisControlConnector = (function() {

        var websocketControl = undefined;
        var port = undefined;
        var x = 0.0;
        var y = 0.0;
        var deltax = 0.0;
        var deltay = 0.0;
        var rot = 0.00;



        return {

            controlConnectionUri: "",

            onMove: function(xnew, ynew) {

                deltax = xnew - x;

                if (deltax > 2.0 || deltax < -2.0) {
                    x = xnew;
                } else {
                    //console.log("x: " + deltax);
                    x = xnew;
                    //console.log("x: " + deltax);
                    rot = rot + deltax * 0.001;
                }

            },

            sendRotation() {
                
               TRI_Frontend.VisControlConnector.doSend('{"req":{"params":{"direction":{"x": ' +rot+ ',"y":0,"z":0}},"rid":108,"sid":1},"type":"MoveCamera"}');
                rot = 0.0;
                
                
                //TRI_Frontend.VisControlConnector.doSend('{"type": "SetIsoValue", "req": { "rid": 1, "sid": 3, "params": { "surfaceIndex": 0, "isovalue": ' + rot + ' } } }');
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
                //TRI_Frontend.VisControlConnector.doSend('{"type": "InitContext", "req": { "rid": 1, "sid": 3, "params": { "dummy": 1} } }');
                
                TRI_Frontend.VisControlConnector.doSend('{"type": "SetIsoValue", "req": { "rid": 1, "sid": 3, "params": { "surfaceIndex": 0, "isovalue": 0.0 } } }');

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
                } catch (error) {
                    console.log("ERROR: Error while parsing Message response to JSON")
                }
                console.log("got sth in control channel:");
            },

            onError: function(evt) {
                console.log("Websocket Error: " + evt.data);
                $("#div_connect_failure").fadeIn('slow', TRI_Frontend.hideConnectMessage);
            },

            doSend: function(message) {
                console.log("Sent: " + message);
                websocketControl.send(message);
            }
        };
    })();



    // NodeConnector handles the WS connection and defines the behaviour
    TRI_Frontend.StreamConnector = (function() {

        var websocketStream = undefined;
        var port = undefined;
        var rot = 0.0;



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
                document.getElementById("myCanvas").style["display"] = "block";
                document.getElementById("myCanvas").style["border"] = "0px";
                document.getElementById("myCanvas").style["margin"] = "0px";
                document.getElementById("myCanvas").style["padding"] = "0px";

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
                console.log("gim in!");
                //console.log(evt.data);

                img.onload = function() {
                        //console.log("drawing...");
                        ctx.drawImage(img, 0, 0);
                    }
                    //console.log("replacing image source");

                img.src = URL.createObjectURL(evt.data);
                TRI_Frontend.VisControlConnector.sendRotation();
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