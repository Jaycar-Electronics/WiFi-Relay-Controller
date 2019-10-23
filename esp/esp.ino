#include <ESP8266WebServer.h>
#include <FS.h>

// Web server to serve our app
ESP8266WebServer server(80);
IPAddress serverIP(1, 0, 0, 1);

//another define bunch
//similar to the uno code
//#define D(...) Serial.println(__VA_ARGS__);
//#define D(...)

const long serial_baud = 115200;

void setup()
{
  Serial.begin(serial_baud);
  D("Starting")
  SPIFFS.begin();
  // setup a new hotspot, no password
  setupHotspot("Relay Controller");

  server.on("/relay", sendRelay);
  server.onNotFound(searchFileSystem);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void setupHotspot(const char *SSID)
{

  WiFi.mode(WIFI_AP);
  //WiFi.begin(SSID); //connect to some network
  //WiFi.softAPConfig(serverIP, serverIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID);
  // todo: add a timeout?
  D("Created soft AP called")
  D(SSID)
  D(WiFi.softAPIP())
}

void sendRelay()
{
  const String relayString = server.arg("relay");
  const String modeString = server.arg("mode");

  if (relayString == "")
  {
    server.send(404, "text/plain", "No relay arg");
    return;
  }

  if (modeString == "")
  {
    server.send(404, "text/plain", "No mode arg");
    return;
  }

  //we have a relay and a mode; let's work on them
  short relay = relayString.toInt();

  if (relay < 0 || relay > 7)
  {
    server.send(500, "text/plain", "Relay out of range");
    return;
  }

  char mode = modeString.equals("activate") ? 'a' : 'd';

  //send this to the UNO
  Serial.write('>');
  Serial.write('0' + relay);
  Serial.write(mode);

  //we send a response to the web-app
  server.send(200, "text/plain", "OK");
}

// ---------------------------------------------------------------------------
void searchFileSystem()
{
  //server.uri() is the param accepted; ie:
  //    http://10.20.30.40/somefile.txt - uri is /somefile.txt
  // we will put it into a string for the string utility functions

  D("Calling filesystem with")
  D(server.uri())
  String filepath = server.uri();

  if (filepath.endsWith("/")) //is this a folder?
  {
    filepath += "index.html"; //index page of the folder being accessed
  }

  if (SPIFFS.exists(filepath))
  {
    String contentType = getFileContentType(filepath);

    File fp = SPIFFS.open(filepath, "r");
    server.client().setNoDelay(true);

    server.streamFile(fp, contentType);
  }
  else
  {
    Serial.printf("%s was not found, plaintext response", filepath.c_str());
    server.send(404, "text/plain", filepath + " File not found, have you uploaded data to the ESP correctly?");
  }
}

String getFileContentType(String &filepath)
{

#define FILE_MATCH(fp, ret)  \
  if (filepath.endsWith(fp)) \
    return ret;

  //got the following from:
  //https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header
  FILE_MATCH(".html", "text/html")
  FILE_MATCH(".txt", "text/plain")
  FILE_MATCH(".css", "text/css")
  FILE_MATCH(".js", "application/javascript")

  // the following is provided as a "just in case" -
  // it doesn't hurt to include them, but we haven't used them in this project (yet)
  FILE_MATCH(".mp4", "audio/mpeg")
  FILE_MATCH(".jpg", "image/jpeg")
  FILE_MATCH(".png", "image/png")

  //at the very least just return something
  return "text/plain";
}

// -- Macro definitions -----------------

// --------------------------------------
