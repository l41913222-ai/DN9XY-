#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN 14
#define DATA_PIN 13
#define CS_PIN 15

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

unsigned long baseEpoch = 0;
unsigned long baseMillis = 0;
String customText = ""; // Speicher für deinen eigenen Text
bool timeSynced = false;
int step = 0;

void setup() {
  P.begin();
  P.setIntensity(2);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Smart-Matrix-Sync");
  dnsServer.start(DNS_PORT, "*", apIP);
  
  server.on("/", handleRoot);
  server.on("/sync", handleSync);
  server.on("/settext", handleSetText);
  server.begin();
}

void handleRoot() {
  String html = "<html><body><h2>Smart Matrix Control</h2>";
  html += "<button onclick=\"fetch('/sync?e='+Math.floor(Date.now()/1000)).then(()=>alert('Time Sent!'))\">SYNC TIME</button><br><br>";
  html += "<form action='/settext'>Text: <input type='text' name='msg'><input type='submit' value='SEND'></form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSync() {
  baseEpoch = server.arg("e").toInt();
  baseMillis = millis();
  timeSynced = true;
  customText = ""; // Zeit-Sync löscht Custom Text Modus
  server.send(200, "text/plain", "OK");
}

void handleSetText() {
  customText = server.arg("msg");
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  
  if (customText != "") {
    // Zeigt den Custom Text als Laufschrift
    if (P.displayAnimate()) {
      P.displayText(customText.c_str(), PA_LEFT, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayReset();
    }
  } else if (!timeSynced) {
    // Zeigt die geführte Anleitung als Laufschrift
    if (P.displayAnimate()) {
      P.displayClear();
      if (step == 0) P.displayText("1. CONNECT WIFI", PA_LEFT, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      else if (step == 1) P.displayText("2. OPEN BROWSER", PA_LEFT, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      else if (step == 2) P.displayText("3. SYNC TIME", PA_LEFT, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      step = (step + 1) % 3;
      P.displayReset();
    }
  } else {
    // Zeigt Uhrzeit
    unsigned long now = baseEpoch + ((millis() - baseMillis) / 1000);
    char buf[12];
    sprintf(buf, "%02d:%02d:%02d", (now/3600)%24, (now/60)%60, now%60);
    P.setTextAlignment(PA_CENTER);
    P.print(buf);
  }
  P.displayAnimate();
}