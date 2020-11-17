var ADXL345_Initialized = true;
document.addEventListener("DOMContentLoaded", function(){
    DrawLevel("X");
	DrawLevel("Y");
	var slider = document.getElementById("ThresholdSlider");
	slider.oninput = function() {		
		document.getElementById("SliderValue").innerHTML = this.value;
	};
	slider.onchange = function() {
		var oRequest = new XMLHttpRequest();
		var sURL  = '/threshold?v=';
		sURL += this.value
		oRequest.open("GET",sURL,true);	
		oRequest.onerror = function (e) {SetOutput("Threshold failed!", true);};
		oRequest.send(null);		
	};
	GetSetup();
});
function GetLevel(){
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
function Valuation(){
	var oRequest = new XMLHttpRequest();
	var sURL  = '/valuation?x=';
	sURL += document.getElementById("ValutationX").value;
	sURL += '&y=';
	sURL += document.getElementById("ValutationY").value;
	oRequest.open("GET",sURL,true);	
	oRequest.onerror = function (e) {SetOutput("Valuation failed!", true);};
	oRequest.send(null);
}
function InvertAxis(){
	var oRequest = new XMLHttpRequest();
	var sURL  = '/invertation?v=';
	sURL += document.getElementById("InvertAxis").checked ? '1' : '0';	
	oRequest.open("GET",sURL,true);	
	oRequest.onerror = function (e) {SetOutput("Invertation failed!", true);};
	oRequest.send(null);
}
function GetSetup(){
	var oRequest = new XMLHttpRequest();
	var sURL  = '/setup';	
	oRequest.open("GET",sURL,true);
	oRequest.onload = function (e) {
		if(oRequest.readyState === 4 && oRequest.status === 200){
			var arr = oRequest.responseText.split("|");	
			ADXL345_Initialized = arr[0];
			document.getElementById("ValutationX").value = arr[1];
			document.getElementById("ValutationY").value = arr[2];
			document.getElementById("InvertAxis").checked = arr[3];
		}
	}
	oRequest.onerror = function (e) {SetOutput("Setup failed!", true);};
	oRequest.send(null);
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
	dot.style.top = can.getBoundingClientRect().top + (can.height / 2 - dot.clientHeight / 2);
	dot.style.left = can.getBoundingClientRect().left + (can.width / 2 - dot.clientWidth / 2);	
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
	let colour = hsl_col_perc(value, 120, 0);//Green -> Red
	element.style.backgroundColor = colour;
}
function hsl_col_perc(percent, start, end) {
  var a = percent / 100,
      b = (end - start) * a,
  		c = b + start;
  // Return a CSS HSL string
  return 'hsl('+c+', 100%, 50%)';
}
function SetSliderVisibility(){	
	document.getElementById("SlideContainer").style.display = "block";
}
function SetOutput(text, error){
	var output = document.getElementById("Output");
	output.innerHTML = text;
	output.style.color = error? "#FF0000" : "#008CBA";
}
function CloseModal() {  
  document.getElementById("SlideContainer").style.display = "none";
}