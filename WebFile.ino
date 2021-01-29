String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".jpg"))
    return "image/jpg";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

void handleFileRead() {
  String path = webServer.uri();
  Serial.println("Handle FileRead: " + path);

  // If a folder is requested, send index.html
  if (path.endsWith("/"))
    path.concat("index.html");

  // If request is captive request, followed with a GUID
  if(path.startsWith("/generate_204")){    
    redirect();
    return;
  }

  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    String fileSize = String(file.size());
    //Check File "Version" (Size) is still the same, otherwise sumbit it
    if (ProcessETag(fileSize.c_str())) {
      file.close();
      return;
    }
    size_t sent = webServer.streamFile(file, getContentType(path));
    file.close();
    Serial.print(String("\tSent file: ") + path);
    Serial.println(" " + String(sent));
    return;
  }

  handleNotFound();
  Serial.println(String("\tFile Not Found: ") + path);
}

File fsUploadFile;
void handle_fileupload() {
  HTTPUpload& upload = webServer.upload();

  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;

    Serial.print("handleFileUpload: ");
    Serial.println(filename);

    if (SPIFFS.exists(filename)) {
      Serial.println(F("\tFile Deleted"));
      SPIFFS.remove(filename);
    }

    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      Serial.print("handleFileUpload Size: ");
      Serial.println(upload.totalSize);
      // Redirect the client to the root page
      webServer.sendHeader("Location", "/");
      webServer.send(303);
    } else {
      webServer.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

