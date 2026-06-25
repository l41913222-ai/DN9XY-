#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CS_PIN 15

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
ESP8266WebServer server(80);

String callsign = "DN9XY";
String customText = "CQ CQ CQ";
bool customEnabled = false;

unsigned long baseEpoch = 0;
unsigned long baseMillis = 0;

char displayBuffer[64];
bool modeChanged = false;

void startCallsign()
{
  P.displayClear();
  P.displayText(
    callsign.c_str(),
    PA_LEFT,
    45,
    0,
    PA_SCROLL_LEFT,
    PA_SCROLL_LEFT
  );
}

void startCustom()
{
  P.displayClear();
  P.displayText(
    customText.c_str(),
    PA_LEFT,
    45,
    0,
    PA_SCROLL_LEFT,
    PA_SCROLL_LEFT
  );
}

void handleRoot()
{
  String html;

  html += "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>DN9XY Matrix</title>";
  html += "</head><body>";

  html += "<h2>DN9XY Matrix</h2>";

  html += "<h3>Rufzeichen</h3>";
  html += "<form action='/setcall'>";
  html += "<input type='text' name='call' value='" + callsign + "'>";
  html += "<input type='submit' value='Speichern'>";
  html += "</form>";

  html += "<h3>Custom Nachricht</h3>";
  html += "<form action='/setmsg'>";
  html += "<input type='text' name='msg' value='" + customText + "'>";
  html += "<input type='submit' value='Speichern'>";
  html += "</form>";

  html += "<br>";

  if (customEnabled)
  {
    html += "<a href='/customoff'><button>Custom AUS</button></a>";
  }
  else
  {
    html += "<a href='/customon'><button>Custom EIN</button></a>";
  }

  html += "<br><br>";
  html += "<button onclick='setTime()'>Zeit vom Handy übernehmen</button>";

  html += R"rawliteral(
<script>
function setTime()
{
  let epoch = Math.floor(Date.now()/1000);

  fetch('/settime?epoch=' + epoch)
  .then(r => r.text())
  .then(t => alert(t));
}
</script>
)rawliteral";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSetCall()
{
  if (server.hasArg("call"))
  {
    callsign = server.arg("call");
    callsign.toUpperCase();

    if (!customEnabled)
      startCallsign();
  }

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleSetMsg()
{
  if (server.hasArg("msg"))
  {
    customText = server.arg("msg");

    if (customEnabled)
      startCustom();
  }

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleCustomOn()
{
  customEnabled = true;
  startCustom();

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleCustomOff()
{
  customEnabled = false;
  modeChanged = false;

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleSetTime()
{
  if (server.hasArg("epoch"))
  {
    baseEpoch = strtoul(server.arg("epoch").c_str(), NULL, 10);
    baseMillis = millis();

    server.send(200, "text/plain", "Zeit gesetzt");
    return;
  }

  server.send(400, "text/plain", "Fehler");
}

void setup()
{
  P.begin();
  P.setIntensity(2);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("DN9XY_MATRIX", "12345678");

  server.on("/", handleRoot);
  server.on("/setcall", handleSetCall);
  server.on("/setmsg", handleSetMsg);
  server.on("/customon", handleCustomOn);
  server.on("/customoff", handleCustomOff);
  server.on("/settime", handleSetTime);

  server.begin();

  startCallsign();
}

void loop()
{
  server.handleClient();

  if (customEnabled)
  {
    if (P.displayAnimate())
      P.displayReset();

    return;
  }

  if (baseEpoch == 0)
  {
    P.print("ZEIT SETZEN");
    return;
  }

  unsigned long epoch =
      baseEpoch + ((millis() - baseMillis) / 1000);

  int sec  = epoch % 60;
  int min  = (epoch / 60) % 60;
  int hour = (epoch / 3600) % 24;

  if (sec < 10)
  {
    if (!modeChanged)
    {
      startCallsign();
      modeChanged = true;
    }

    if (P.displayAnimate())
      P.displayReset();
  }
  else
  {
    if (modeChanged)
    {
      P.displayClear();
      modeChanged = false;
    }

    sprintf(displayBuffer,
            "%02d:%02d:%02d",
            hour,
            min,
            sec);

    P.setTextAlignment(PA_CENTER);
    P.print(displayBuffer);

    P.displayAnimate();
  }
}
