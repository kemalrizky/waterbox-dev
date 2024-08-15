#include "ConfigWebServer.h"


const char index_html_start[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Calibration Factor: 
)rawliteral";

const char index_html_end[] PROGMEM = R"rawliteral(
    <input type="text" name="calibFactor">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

const char *PARAM_INPUT = "calibFactor";

void ConfigWebServer::init()
{
  configStorage.init();

  if (!internetHandler.checkConnection()) {
    internetHandler.connect();
  }

  server = new AsyncWebServer(90);
  endPointRegister(server);
  server->begin();
}

void ConfigWebServer::notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void ConfigWebServer::endPointRegister(AsyncWebServer *_server)
{
    _server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { 
                    char index_html[500] = {};
                    char calibFactorStr[10];
                    sprintf(calibFactorStr, "%f", configStorage.getCalibrationFactor());

                    strcat(index_html, index_html_start);
                    strcat(index_html, calibFactorStr);
                    strcat(index_html, index_html_end);
                    request->send_P(200, "text/html", index_html); });

    // Send a GET request to <IP>/get?message=<message>
    _server->on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
                {
    
    String inputMessage;
    String inputParam;
    
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      inputParam = PARAM_INPUT;

      configStorage.setCalibrationFactor(inputMessage.toFloat());
      waterflowSensorHandler.setCalibrationFactor(configStorage.getCalibrationFactor());
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"); });

    server->onNotFound(&ConfigWebServer::notFound);
}