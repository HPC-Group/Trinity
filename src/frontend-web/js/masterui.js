/****************************************************************
* Copyright (C) Jürgen Grüninger 2016
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
* 
****************************************************************/


var MUI_MasterUI = {

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
	},
	
	connect : function ()
	{
		MUI_MasterUI.Connector.initWS();
        MUI_MasterUI.password = $("#connect_password").val()
	},
	
    /*
	buildApplicationList : function (queryApplicationResult)
	{
        //clear list
        $("#panel_requirements").hide();
        MUI_MasterUI.slideCount = 0;
        var slides = $("#carousel_slides").children();
        slides[0].classList.add("active");
        var indicators = $("#carousel_indicators").children();
        indicators[0].classList.add("active");
        for ( var i=1; i<slides.length;i++)
        {
            slides[i].remove();
            indicators[i].remove();
        }
        
        var applicationsArray = queryApplicationResult.applications;
        console.log("Building applicatons list");
        if (applicationsArray.length > 0) 
        {
            for (var i=0; i<applicationsArray.length; i++)
            {
                var imgpath = undefined;
                for (var j=0; j<MUI_MasterUI.applicationTypes.length; j++)
                {
                    if (applicationsArray[i].applicationTypeID == MUI_MasterUI.applicationTypes[j].id)
                    {
                        imgpath = MUI_MasterUI.applicationTypes[j].imagePath;
                        break;
                    }
                }
                
                if (imgpath == undefined)
                {
                    if (i%2) {
                        imgpath = "imagevis_fractal.png";
                    } else {
                        imgpath = "imagevis_fractal_back.png";
                    }
                    MUI_MasterUI.addApplicationSlide(applicationsArray[i].applicationTypeID, applicationsArray[i].id, imgpath);
                } else {
                    MUI_MasterUI.addApplicationSlide(applicationsArray[i].applicationTypeID, applicationsArray[i].id, imgpath);
                }
            }
            $("#apps").show();
            window.scrollTo(0,document.body.scrollHeight);
        }
	},
    
    setApplicationTypes : function (queryTypesResult) 
    {
        MUI_MasterUI.applicationTypes = queryTypesResult.types;
        
        MUI_MasterUI.Connector.sendRequest("queryApplications");
    },
    
    setDevices : function (queryTypesResult) 
    {
        MUI_MasterUI.deviceDict = new MUI_MasterUI.Dictionary();
        
        var regDevices = queryTypesResult.devices;
        
        for (var i=0; i<regDevices.length; i++)
        {
            MUI_MasterUI.deviceDict.add(regDevices[i].id, regDevices[i].deviceTypeID);
        }
    },
    
    setDeviceTypes : function (queryTypesResult) 
    {
        MUI_MasterUI.deviceTypes = queryTypesResult.types;
        
    },
	
	applicationSelected : function (name, description)
	{
        //empty requirements, reset requirements row counter and disable start ui button
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
        }
	},
    */
	
	hideConnectMessage : function () 
	{
		MUI_MasterUI.connectFadeoutTimeout = setTimeout( function () 
			{
				 $("#div_connect_success").fadeOut('slow');
				 $("#div_connect_failure").fadeOut('slow');
			},
			5000
			);
		
	},
	
    /*
	addApplicationSlide : function(name, description, image)
	{
		// WORKAROUND for Bootstrap Carousel
		// First slide needs "active" as additional class and needs to be present during webpage generation -> keep it in html and modify first item
		if(MUI_MasterUI.slideCount == 0) 
        {
			$("#carousel_slides").find("h1")[0].innerHTML = name;
			$("#carousel_slides").find("p")[0].innerHTML = description;
            $("#carousel_slides").find("img")[0].src = "img/mui/"+ image;
			var linknode = $("#carousel_slides").find("a")[0];
			linknode.setAttribute("onclick", "MUI_MasterUI.applicationSelected('"+name+"','"+description+"')");
		} else {
            var parameterString = "\""+name+"\",\""+description+"\"";
            //TODO: Insert correct image
			$("#carousel_slides").append("<div class='item'> \
				<img src='img/mui/"+ image +"' alt='Chania'> \
				<div class='container'> \
					<div class='carousel-caption'> \
						<h1>"+name+"</h1> <p>"+description+"</p> <p> \
						<a class='btn btn-lg btn-primary' role='button' onclick='MUI_MasterUI.applicationSelected("+parameterString+")'>Select</a></p> \
					</div> \
				</div> \
			</div>");
			$("#carousel_indicators").append("<li data-target='#applicationCarousel' data-slide-to='"+MUI_MasterUI.slideCount+"'></li>");
		}
		
		MUI_MasterUI.slideCount++;
	},
    
    queryMuiProposals : function(appRequirements, appName)
    {
		if (appRequirements.length > 0) 
        { 
            console.log("Querying Proposals");
            MUI_MasterUI.proposalRequestsNeeded = appRequirements.length;
            for (var i = 0; i<appRequirements.length; i++) 
            {
                //get devices matching the requirements
                var applicationRequirement = { applicationID : appName, requirementID : appRequirements[i].id };
                
                MUI_MasterUI.Connector.sendRequest('generateProposals', '{ "applicationRequirement" : ' + JSON.stringify(applicationRequirement) + ' }');
            }
        }
    },
    
    checkProposalCount : function (proposal, pastParameters)
    {
        var params = JSON.parse(pastParameters);
        
        var deviceList = [];
        
        for (var i=0; i<proposal.proposals.length;i++)
        {
            var hasDevice = false;
            for(var j=0; j<deviceList.length; j++)
            {
                if(proposal.proposals[i].deviceID == deviceList[j].deviceID)
                {
                    hasDevice = true;
                }
            }
            
            if (!hasDevice)
            {
                deviceList[deviceList.length] = {deviceID : proposal.proposals[i].deviceID, capabilityIDs : [proposal.proposals[i].capabilityID] };
            } else {
                for(var j=0; j<deviceList.length;j++)
                {
                    if (deviceList[j].deviceID == proposal.proposals[i].deviceID)
                    {
                        var hasCapabilityID = false;
                        for(var k=0; k<deviceList[j].capabilityIDs.length; k++)
                        {
                            if (deviceList[j].capabilityIDs[k] == proposal.proposals[i].capabilityID)
                            {
                                hasCapabilityID = true;
                            }
                        }
                        if (!hasCapabilityID)
                        {
                            deviceList[j].capabilityIDs[deviceList[j].capabilityIDs.length] = proposal.proposals[i].capabilityID;
                        }
                    }
                }
            }
        }
        
        MUI_MasterUI.proposals[MUI_MasterUI.proposals.length] = {applicationID:params.applicationRequirement.applicationID, requirementID:params.applicationRequirement.requirementID, deviceList:deviceList};
        
        MUI_MasterUI.addRequirementPanel(params.applicationRequirement.requirementID, deviceList);
        
        MUI_MasterUI.proposalRequestsCount++;
        
        if (MUI_MasterUI.proposalRequestsCount == MUI_MasterUI.proposalRequestsNeeded)
        {
            MUI_MasterUI.showRequirementList();
        }
        
    },
	
	showRequirementList : function() 
	{
        console.log("showing Requirements list");
        //Apply flat UI style to the select-boxes
        $('[data-toggle="select"]').select2({
            formatResult: MUI_MasterUI.formatDropdown,
            formatSelection: MUI_MasterUI.formatDropdown,
            escapeMarkup: function(m) { return m; }
        });
        
        //Apply onchange behaviour to device selectors
        $("select[id*='dropdownMenu_device_']").on('change', function(){
            var selected = $(this).find("option:selected").val();
            var capabilitySelectorNode = $(this).parent().find("select")[1];

            if (selected != 0)
            {
                //get device index
                var proposalIndex = undefined;
                for (var i=0; i<MUI_MasterUI.proposals.length; i++)
                {
                    if (MUI_MasterUI.proposals[i].requirementID == $(this).parent().find("h3").text())
                    {
                        proposalIndex = i;
                        break;
                    }
                }
                var deviceIndex = undefined;
                for (var i=0; i<MUI_MasterUI.proposals[proposalIndex].deviceList.length; i++)
                {
                    if (MUI_MasterUI.proposals[proposalIndex].deviceList[i].deviceID == $(this).find("option:selected").text())
                    {
                        deviceIndex = i;
                        break;
                    }
                }
                
                capabilitySelectorNode.innerHTML = "<option value='0' selected>Choose capability</option>";
                var capabilityHTML = "";
                for (var i=0; i<MUI_MasterUI.proposals[proposalIndex].deviceList[deviceIndex].capabilityIDs.length; i++)
                {
                    if (!i)
                    {
                        capabilityHTML += "<option value='"+ (i+1) +"'>"+ MUI_MasterUI.proposals[proposalIndex].deviceList[deviceIndex].capabilityIDs[i] +"</option>";
                    } else {
                        capabilityHTML += "<option value='"+ (i+1) +"'>"+ MUI_MasterUI.proposals[proposalIndex].deviceList[deviceIndex].capabilityIDs[i] +"</option>";
                    }

                }

                capabilitySelectorNode.innerHTML += capabilityHTML;
                
                capabilitySelectorNode.disabled = false;
            } else {
                capabilitySelectorNode.disabled = true;
                $(this).parent().find("select").eq(1).val("0").trigger("change");
            }
        });
        
        
        //Apply onchange behaviour to capability selectors
        $("select[id*='dropdownMenu_deviceCapability_']").on('change', function(){
            var capabilitySelectorNodes = $("select[id*='dropdownMenu_deviceCapability_']");
            
            var oneHaveSelection = false;
            for (var i=0; i<capabilitySelectorNodes.length; i++)
            {
                if (capabilitySelectorNodes[i].value != 0)
                {
                    oneHaveSelection = true;
                }
            }
            
            if (oneHaveSelection)
            {
                // ENABLE start UI button
                $('#btnStartUI').prop('disabled', false);
            } else {
                // DISABLE start UI button
                $('#btnStartUI').prop('disabled', true);
            }
            
        });
        
        //Show the constructed panel
        $("#panel_requirements").show();
        window.scrollTo(0,document.body.scrollHeight);
        
	},
	
	addRequirementPanel : function(name, devices)
	{
		var elementsPerRow = 4;
		
		if ((MUI_MasterUI.requirementPanelCount) % elementsPerRow == 0 ) 
        {
			console.log("adding row");
			$("#container_requirements").append("<div class='row show-grid'></div>");
		}
        
        if ((MUI_MasterUI.requirementPanelCount % 2 == 0) && MUI_MasterUI.requirementPanelCount > 0) 
        {
			console.log("adding clearfix");
			$("#container_requirements").find(".row").last().append("<div class='clearfix visible-sm-block visible-md-block'></div>");
		}
        
        
        var deviceHTML = "";
        for (var i=0; i<devices.length; i++)
        {
            if (!i)
            {
                deviceHTML += "<option value='"+ (i+1) +"'>"+ devices[i].deviceID +"</option>";
            } else {
                deviceHTML += "<option value='"+ (i+1) +"'>"+ devices[i].deviceID +"</option>";
            }

        }
        
		$("#container_requirements").find(".row").last().append("\
			<div class='panel panel-default col-sm-6 col-md-6 col-lg-3' align='center'>\
			  <h3>"+name+"</h3>\
              <select id='dropdownMenu_device_"+(MUI_MasterUI.requirementPanelCount+1)+"'class='form-control select select-primary' data-toggle='select'>\
                  <option value='0' selected>Choose device</option>\
                  "+ deviceHTML +"\
              </select>\
              <p></p>\
              <select id='dropdownMenu_deviceCapability_"+(MUI_MasterUI.requirementPanelCount+1)+"' class='form-control select select-primary' data-toggle='select' disabled>\
                  <option value='0' selected>Choose capability</option>\
              </select>\
              <p></p>\
			</div>");
            

		MUI_MasterUI.requirementPanelCount++;
	},
    
    startUI : function ()
    {
            
            var uiObject = {ui : {}, password: MUI_MasterUI.password};
            uiObject.ui = {applicationID: MUI_MasterUI.selectedApplicationDescription, elements: []};
            
            var deviceSelectionNodes = $("select[id*='dropdownMenu_device_']");
            for (var i = 0; i<deviceSelectionNodes.length; i++)
            {
                var selectstring = "#s2id_dropdownMenu_device_" + (i+1);
                var dropdownNodes = $(selectstring).parent().find("select");
                
                if (dropdownNodes[1].value != 0) {
                    var element = { deviceCapability: {capabilityID: dropdownNodes[1].selectedOptions[0].innerText, deviceID : dropdownNodes[0].selectedOptions[0].innerText} , requirementID:$(selectstring).parent().find("h3").text()};
                    
                    uiObject.ui.elements[uiObject.ui.elements.length] = element;
                }
            }

            MUI_MasterUI.Connector.sendRequest('startUI', JSON.stringify(uiObject));
    },
    
    buildUiList : function(uisQueryResult) 
    {
        var uiArray = uisQueryResult.uis;

        //clear list before computing a new one
        $("#uiModalList").empty();
        
        if (uiArray.length > 0)
        {   
            for (var i = 0; i<uiArray.length; i++)
            {
                var idstring = '"'+uiArray[i].id+'"';
                $("#uiModalList").append("\
                    <li class='list-group-item'>\
                        <label>"+ uiArray[i].applicationID +", UI-ID: "+ uiArray[i].id +"</label>\
                        <span class='pull-right'><button type='button' class='btn btn-sm btn-primary' onclick='MUI_MasterUI.stopUI("+idstring+")'>UI stoppen</button></span>\
                    </li>\
                ");
            }
        }
        
    },
    
    stopUI : function (uiId)
    {
        //TODO: Correct UI-id handling and stopping
        var paramObj = {id: uiId, password: MUI_MasterUI.password};
        MUI_MasterUI.Connector.sendRequest("stopUI", JSON.stringify(paramObj));
    },
    
    formatDropdown : function (o) {
        
        var deviceType = "";
        if (MUI_MasterUI.deviceDict != undefined)
        {
            deviceType = MUI_MasterUI.deviceDict.getValue(o.text);
        }
        
        switch (deviceType) {
            case "GameController":
                return "<img src='img/mui/gamepad.png'>&nbsp;" + o.text;
                break;
            case "Mouse":
                return "<img src='img/mui/mouse.png'>&nbsp;" + o.text;
                break;
            case "Keyboard":
                return "<img src='img/mui/keyboard.png'>&nbsp;" + o.text;
                break;
            case "Leap":
                return "<img src='img/mui/leap_motion.png'>&nbsp;" + o.text;
                break;
            case "Kinect2":
                return "<img src='img/mui/kinect.png'>&nbsp;" + o.text;
                break;
            default:
                return o.text; // optgroup
                break;
        }
        
    }
    */

}


// Connector handles the WS connection and defines the behaviour
MUI_MasterUI.Connector = (function (){
	
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
			//messageDict = new MUI_MasterUI.Dictionary();
            //parameterDict = new MUI_MasterUI.Dictionary();
			MUI_MasterUI.Connector.wsUri = "ws://"+$("#proc_ip")[0].value+":"+$("#proc_port")[0].value+"/";

			websocket = new WebSocket(MUI_MasterUI.Connector.wsUri);
			websocket.onopen = function(evt) { MUI_MasterUI.Connector.onOpen(evt) };
			websocket.onclose = function(evt) { MUI_MasterUI.Connector.onClose(evt) };
			websocket.onmessage = function(evt) { MUI_MasterUI.Connector.onMessage(evt) };
			websocket.onerror = function(evt) { MUI_MasterUI.Connector.onError(evt) };
		
		},

		onOpen : function(evt) 
		{
			console.log("Websocket CONNECTED to " + MUI_MasterUI.Connector.wsUri);
			$("#div_connect_success").fadeIn('slow', MUI_MasterUI.hideConnectMessage);
            console.log("sending init renderer request");
            MUI_MasterUI.Connector.sendRequest("InitRenderer");
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
            } catch (error) {
                console.log("ERROR: Error while parsing Message response to JSON")
            }


			if (messageResult.result !== undefined)
            {
				//do a lookup to find out, to which type of request this id belongs to
				var responseType = messageDict.getValue(messageResult.id);
			
				switch (responseType) 
				{
					case "queryGates":
                        console.log("Gates list received");
                        //messageResult.result;
						break;
					case "queryDevices":
                        console.log("Devices received");
                        MUI_MasterUI.setDevices(messageResult.result);
						break;
                    case "queryDeviceTypes":
                        console.log("Device types received");
                        MUI_MasterUI.setDeviceTypes(messageResult.result);
                        break;
                    case "queryApplications":
                        console.log("Application list received");
						MUI_MasterUI.buildApplicationList(messageResult.result);
						break;
					case "generateProposals":
                        var pastParameters = parameterDict.getValue(messageResult.id);
                        MUI_MasterUI.checkProposalCount(messageResult.result, pastParameters);
						break;
                    case "startUI":
						break;
					case "stopUI":
                        MUI_MasterUI.Connector.sendRequest('queryUIs');
						MUI_MasterUI.applicationSelected(MUI_MasterUI.selectedApplicationName,MUI_MasterUI.selectedApplicationDescription);
						break;
					case "queryUIs":
                        console.log("UI list received");
                        MUI_MasterUI.buildUiList(messageResult.result);
						break;
                    case "queryApplicationTypes":
                        console.log("Application types received");
                        MUI_MasterUI.setApplicationTypes(messageResult.result);
                        break;
					default:
						break;
				}
			
				messageDict.remove(messageResult.id);
                parameterDict.remove(messageResult.id);
			
			} else {
				console.log("ERROR: " + messageResult.error.code + ", " + messageResult.error.message);
                $('#errorBody').text(messageResult.error.code + ", " + messageResult.error.message);
                $('#errorModal').modal('show');
			}
		},

		onError : function(evt)
		{
			console.log("Websocket Error: " + evt.data);
			$("#div_connect_failure").fadeIn('slow', MUI_MasterUI.hideConnectMessage);
		},

		doSend : function(message)
		{
			console.log("Sent: " + message);
			websocket.send(message);
		},
	
		sendRequest : function(requestType, parameter) 
		{
			var requestTypes = ["InitRenderer"];
			var parameteredTypes = ["generateProposals", "startUI", "stopUI"];
			if (requestTypes.indexOf(requestType) != -1 )
            {

                    console.log("Sending---: ");
					if (parameteredTypes.indexOf(requestType) == -1)
                    {
                        // look like type: InitRenderer; rid: 1; sid: 0; params: { protocol: tcp.prefixed; rendertype: SimpleRenderer; fileid: FractalData@3; ioendpoint: tcp.prefixed:127.0.0.1:6678; streamingparams: { xres: 800; yres: 600 } }
                        console.log("Sending---: " + requestType);
						//MUI_MasterUI.Connector.doSend('{"type": "InitRenderer", "rid": "1", "sid": "0", "params": "'{ "protocol": "tcp.prefixed", "rendertype": "SimpleRenderer", "fileid": "FractalData@3", "ioendpoint": "tcp.prefixed:127.0.0.1:6678", "streamingparams": "'{ "xres": "800", "yres": "600" }'" }'" }');
                        MUI_MasterUI.Connector.doSend('{"jsonrpc": "2.0"}');
					} else {
                        parameterDict.add(randId, parameter)
						MUI_MasterUI.Connector.doSend('{"jsonrpc": "2.0", "method": "'+requestType+'", "params": '+parameter+', "id": "'+randId+'"}');
					}
			
			}

		}
	};
	
})();

// A dictionary with basic functionality
// !!Caution!! Not typed so it allows different key and value type combinations in the same dictionary
// "Contains"-functionality can be emulated via the getValue function
MUI_MasterUI.Dictionary = function () {
	
	var elements = [];
		
	// Adds an element to the dictionary with the identifying key and the associated value	
	this.add = function (key, value) 
	{
		elements.push( {'key':key, 'value':value} );
	}
	
	// Removes a certain key-value-pair identified by its key
	this.remove = function (key) 
	{
		for (var i = 0; i<elements.length; i++)
        {
			if ( elements[i].key == key )
            {
				elements.splice(i,1);
				return true;
			}
		}
		return undefined;
	}
	
	// Clears the dictionary
	this.clear = function () 
	{
		elements = [];
	}
	
	// Returns the value for the given key, otherwise returns undefined
	this.getValue = function (key) 
	{
		for (var i = 0; i<elements.length; i++)
        {
			if ( elements[i].key == key )
            {
				return elements[i].value;
			}
		}
		return undefined;
	}
	
	// Sets a new value for a specified key, returns undefined if the key does not exist
	this.setValue = function (key, value) 
	{
		for (var i = 0; i<elements.length; i++)
        {
			if ( elements[i].key == key )
            {
				elements[i].value = value;
				return true;
			}
		}
		return undefined;
	}
	
	// Returns the number of elements currently included in the dictionary
	this.elementsCount = function () 
	{
		return elements.length;
	}

}