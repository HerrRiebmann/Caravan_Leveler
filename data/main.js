var ADXL345_Initialized = true;
var PCVersion = false;
document.addEventListener("DOMContentLoaded", function(){
    DrawLevel("X");
	DrawLevel("Y");
	var slider = document.getElementById("ThresholdSlider");
	slider.oninput = function() {
		document.getElementById("SliderValue").innerHTML = this.value;
	};
	SetSetup(false);
});
function GetLevel(){
	//if(PCVersion)	return;
	var oRequest = new XMLHttpRequest();
	var sURL  = '/level';
	oRequest.open("GET",sURL,true);	
	oRequest.onload = function (e) {
	  if (oRequest.readyState === 4) {
		if (oRequest.status === 200) {
			var arr = oRequest.responseText.split("|");
			SetLevel(arr[0], arr[1], arr[2]);
			if(!ADXL345_Initialized){
				ADXL345_Initialized = true;
				DrawLevel("X");
				DrawLevel("Y");
				SetOutput("", false);
			}
		} else {
			SetLevel(0,0,10);
			ADXL345_Initialized = false;
			DrawLevel("X");
			DrawLevel("Y");
			SetOutput(oRequest.responseText, true);
		}
	  }
	};
	oRequest.onerror = function (e) {
		PCVersion = true;
		//Fake values on a computer. Referring to the ESP will alert a cross site scripting error
		SetLevel(Math.floor(Math.random() * 90) * -1, Math.floor(Math.random() * 90), 10);		
		SetOutput("PC random values", false);
	};
	oRequest.send(null);
}
function Calibrate(){
	var oRequest = new XMLHttpRequest();
	var sURL  = '/calibrate';
	oRequest.open("GET",sURL,true);
	oRequest.onload = function (e) {
		if(oRequest.readyState === 4 && oRequest.status === 200){
			SetOutput(oRequest.responseText, false);
			document.getElementById("Calibrate").style.backgroundColor = "#4CAF50";
		}
		else{
			SetOutput(oRequest.responseText, true);	
			document.getElementById("Calibrate").style.backgroundColor = "#f44336";
		}
	};
	oRequest.onerror = function (e) {
		SetOutput("Calibration Error!", true);
		document.getElementById("Calibrate").style.backgroundColor = "#f44336";
	};
	oRequest.send(null);
}

function SetSetup(submitData = true){
	var oRequest = new XMLHttpRequest();
	var sURL  = '/setup';
	if(submitData){
		sURL += '?vx=';
		sURL += document.getElementById("ValutationX").value;
		sURL += '&vy=';
		sURL += document.getElementById("ValutationY").value;
		sURL += '&inv=';
		sURL += document.getElementById("InvertAxis").checked ? '1' : '0';
		sURL += '&ap=';
		sURL += document.getElementById("Accesspoint").checked ? '1' : '0';
		sURL += '&t=';
		sURL += document.getElementById("ThresholdSlider").value;
	}
	oRequest.open("GET",sURL,true);
	oRequest.onload = function (e) {
		if(oRequest.readyState === 4 && oRequest.status === 200){
			var arr = oRequest.responseText.split("|");	
			ADXL345_Initialized = arr[0];
			document.getElementById("ValutationX").value = arr[1];
			document.getElementById("ValutationY").value = arr[2];
			document.getElementById("InvertAxis").checked = arr[3] == '1' ? true : false;
			document.getElementById("Accesspoint").checked = arr[4] == '1' ? true : false;
			if(submitData){
				document.getElementById("SaveBtn").style.backgroundColor = "#00e600";
				ResetControlsDelayed();	
			}
		}
	};
	oRequest.onerror = function (e) {
		SetOutput("Set Setup failed!", true);	
		document.getElementById("SaveBtn").style.backgroundColor = "#FF0000";
	}
		oRequest.send(null);
}
function MeasureValuation() {
	var btn = document.getElementById("MeasureBtn");
	var measuring = btn.innerHTML == "Rec";
	btn.innerHTML = measuring ? "End"	: "Rec";
	btn.style.backgroundColor = measuring ? "#f44336" : "#008CBA";
	var oRequest = new XMLHttpRequest();
	var sURL  = '/valuate';
	oRequest.open("GET",sURL,true);
	oRequest.onload = function (e) {
		if(oRequest.readyState === 4 && oRequest.status === 200){						
			SetOutput(oRequest.responseText, false);
		}
		else{			
			SetOutput("Recording Error", true);
		}
	};
	oRequest.onerror = function (e) {		
		SetOutput("Recording Error", true);
	};
	oRequest.send(null);	
	if(!measuring)		
		SetSetup(false);	
}
function Upload(){
	document.getElementsByClassName('dimmer')[0].style.display = 'block';	
	document.body.style.cursor = 'progress';
	if(!PCVersion)
		document.getElementById("UploadForm").submit();
}
function SetLevel(x,y, threshold){
	document.getElementById("LevelX").value = (x / 10).toFixed(1);
	document.getElementById("LevelY").value = (y / 10).toFixed(1);
	if(document.getElementById("ThresholdValue").innerHTML != threshold){
		document.getElementById("ThresholdSlider").value = threshold;
		document.getElementById("ThresholdValue").innerHTML  = threshold;
		document.getElementById("SliderValue").innerHTML = threshold;
	}
	SetDots("X");
	SetDots("Y");
}
function SetDots(desc){
	var can = document.getElementById("Level"+desc+"Canvas");
	var txt = document.getElementById("Level"+desc);
	var dot = document.getElementById("Dot"+desc);
	dot.style.margin = "0px 0px 0px 0px";
	//Dot + Border (4px)
	dot.style.top = can.getBoundingClientRect().top + (can.height / 2 - (dot.clientHeight + 4) / 2);
	dot.style.left = can.getBoundingClientRect().left + (can.width / 2 - (dot.clientWidth + 4) / 2);
	var value = parseFloat(txt.value);
	var threshold = document.getElementById("ThresholdSlider").value;		
	var max = desc == "X" ? (can.height/ 2 - dot.clientHeight / 2) : (can.width / 2 - dot.clientWidth / 2);
	value = (max / threshold) * value;
	if(value < max * -1)
		value = max * -1;
	if(value > max)
		value = max;
	if(desc == "X")
		dot.style.top = parseInt(dot.style.top, 10) + value + "px";
	if(desc == "Y")
		dot.style.left = parseInt(dot.style.left, 10) + value + "px";
	SetColor(txt, (value / max) * 100);
}
function DrawLevel(desc){
	var c = document.getElementById("Level" + desc + "Canvas");
	var height = c.height;
	var width = c.width;
	var ctx = c.getContext("2d");
	ctx.beginPath();
	ctx.rect(0, 0, width, height);
	ctx.fillStyle = ADXL345_Initialized ? "#CCFF99" : "#FF0000";
	ctx.fillRect(2, 2, width-4, height-4);
	
	var widthTmp = (desc == "Y") ? width / 3 -5 : 0;
	var heightTmp = (desc == "X") ? height / 3 -5 : 0;
	
	ctx.moveTo(widthTmp, heightTmp);
	if(desc == "Y")
		ctx.lineTo(widthTmp, height);
	else
		ctx.lineTo(width, heightTmp);
	
	if(desc == "X")
		heightTmp += 10;
	else
		widthTmp += 10;
	ctx.moveTo(widthTmp, heightTmp);
	if(desc == "Y")
		ctx.lineTo(widthTmp, height);
	else
		ctx.lineTo(width, heightTmp);
	
	widthTmp = (desc == "Y") ? width / 3 * 2 -5 : 0;
	heightTmp = (desc == "X") ? height / 3 * 2 -5 : 0;
	
	ctx.moveTo(widthTmp, heightTmp);	
	if(desc == "Y")
		ctx.lineTo(widthTmp, height);
	else
		ctx.lineTo(width, heightTmp);
	
	if(desc == "X")
		heightTmp += 10;
	else
		widthTmp += 10;
	ctx.moveTo(widthTmp, heightTmp);
	if(desc == "Y")
		ctx.lineTo(widthTmp, height);
	else
		ctx.lineTo(width, heightTmp);
	ctx.stroke();
}
function SetColor(element, value){
	if(value < 0)
		value = value * -1;	
	if(value > 100)
		value = 100;
	//Green -> Red
	var colour = hsl_col_perc(value, 120, 0);
	element.style.backgroundColor = colour;
}
function hsl_col_perc(percent, start, end) {
  var a = percent / 100,
      b = (end - start) * a,
  	  c = b + start;
  // Return a CSS HSL string
  return 'hsl('+c+', 100%, 50%)';
}
function SetSetupVisibility(){	
	document.getElementById("SetupContainer").style.display = "block";
	document.getElementById("SaveBtn").style.backgroundColor = "#008CBA";
}
function SetOutput(text, error){
	var output = document.getElementById("Output");
	output.innerHTML = text;
	output.style.color = error? "#FF0000" : "#008CBA";
	ResetControlsDelayed();
}
function ResetControlsDelayed() {
	setTimeout(ResetControls,3000);
}
function ResetControls() {
	var output = document.getElementById("Output");
	output.innerHTML = "";
	output.style.color = "#008CBA";
	document.getElementById("Calibrate").style.backgroundColor = "#008CBA";
	document.getElementById("SaveBtn").style.backgroundColor = "#008CBA";
}
function CloseModal() {  
  document.getElementById("SetupContainer").style.display = "none";
}