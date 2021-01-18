void WiFiBegin() {
  //Manually change between WiFi and Accesspoint. AP will be used as a fallback, after 5 seconds
  if (useAcessPointMode)
    CreateAccessPoint();
  else
    ConnectToAccessPoint();
  
  webServer.on("/level", handle_level);
  webServer.on("/setup", handle_setup); 
  webServer.on("/calibrate", handle_calibrate);
  webServer.on("/valuate", handle_valuation);
  webServer.on("/upload",  HTTP_POST,[](){ webServer.send(200);}, handle_fileupload);
  webServer.on("/generate_204", handle_root);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  webServer.on("/fwlink", handle_root);   //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  
  webServer.onNotFound(handleFileRead);

  const char* Headers[] = {"If-None-Match"};
  webServer.collectHeaders(Headers, sizeof(Headers) / sizeof(Headers[0]));

  webServer.begin();
  Serial.println(F("HTTP webServer started"));
  delay(100);
}

void ConnectToAccessPoint() {
  WiFi.begin(ssid, password);

  long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (millis() - start > 5000) {
      Serial.println("Wifi not found!");
      CreateAccessPoint();
      return;
    }
  }
  Serial.println();
  Serial.println(F("WiFi connected successfully"));
  Serial.print(F("Got IP: "));
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial
}

void CreateAccessPoint() {
  WiFi.disconnect();
  IPAddress local_ip(8, 8, 8, 8);
  IPAddress gateway(8, 8, 8, 8);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(deviceName);
  delay(500);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(500);
  Serial.print(F("AP IP address: "));
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS webServer redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", local_ip);
}

void handle_root() {
  Serial.println(F("Handle Root"));
  String path = "/index.html";
  if (!SPIFFS.exists(path))
    return;

  File f = SPIFFS.open(path);
  webServer.streamFile(f, "text/html");
  f.close();
}

void handle_level() {
  // /level
  if (!accelInitialized) {
    webServer.send(400, "text/plain", "Gyro not initialized!");
    return;
  }

  //getLevel();
  String txt = String(invertAxis ? levelY : levelX);
  txt.concat("|");
  txt.concat(String(invertAxis ? levelX : levelY));
  txt.concat("|");
  txt.concat(String(levelThreshold));
  webServer.send(200, "text/plain", txt);

  lastMillisClientAvailable = millis();
}

void handle_setup() {
  // /setup

  Serial.println(F("Handle Setup"));

  //With arguments:
  // /setup?x=123&y=321&inv=0&ap=1
  ProcessSetupArguments();

  String txt = String(accelInitialized);
  txt.concat("|");
  txt.concat(String(valuationX));
  txt.concat("|");
  txt.concat(String(valuationY));
  txt.concat("|");
  txt.concat(String(invertAxis));
  txt.concat("|");
  txt.concat(String(useAcessPointMode));
  webServer.send(200, "text/plain", txt);
}

void handle_calibrate() {
  Serial.println(F("Handle Calibration"));
  CalibrateLevel();
  String result = "Calibration OK (";
  result.concat(calibrationX);
  result.concat("/");
  result.concat(calibrationY);
  result.concat(")");
  webServer.send(200, "text/plaint", result);
}

void handle_valuation() {
  Serial.println(F("Handle Valuation"));  
  String result = "Calibration ";
  if(valuationActive)
    result.concat("stopped");
  else {
    minValuationX = 0;
    minValuationY = 0;
    maxValuationX = 0;
    maxValuationY = 0;
    result.concat("started");
  }
  valuationActive = !valuationActive; 
  webServer.send(200, "text/plaint", result);
}
void handleNotFound() {
  Serial.println(F("HandleNotFound"));

  for (uint8_t i = 0; i < webServer.args(); i++) {
    Serial.println(String(F(" ")) + webServer.argName(i) + F(": ") + webServer.arg(i) + F("\n"));
  }

  if (captivePortal())
    return;
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += webServer.uri();
  message += F("\nMethod: ");
  message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += webServer.args();
  message += F("\n");

  for (uint8_t i = 0; i < webServer.args(); i++) {
    message += String(F(" ")) + webServer.argName(i) + F(": ") + webServer.arg(i) + F("\n");
  }
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.send(404, "text/plain", message);
}

boolean captivePortal() {
  Serial.print(F("Captive Check: "));
  Serial.println(webServer.hostHeader());
  if (!isIp(webServer.hostHeader())) {
    Serial.println("Request redirected to captive portal");
    webServer.sendHeader("Location", String("http://") + toStringIp(webServer.client().localIP()), true);
    webServer.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    webServer.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}
