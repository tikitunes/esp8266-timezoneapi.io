#include <ESP8266WiFi.h>
#include <Time.h>
#include "tzIO.h"

const char *ssid = "";
const char *password = "";

TimezoneAPI tzapi;

void setup() {
  Serial.begin(115200);
  configWiFi();
  tzapi.getData();
  configTime();
  Serial.println("");
  Serial.println("      Time Zone Offset: " + tzapi.getOffset());
  Serial.println("Time Zone Setup String: " + tzapi.getTZinfo());
  time_t tnow = time(nullptr);
  String localTime = String(ctime(&tnow));
  Serial.println("          Current Time: " + localTime);
}

void loop() {
  delay(1000 * 5);
}

void configWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
}

void configTime() {
  String tzinfo = tzapi.getTZinfo();
  const char *tz = tzinfo.c_str();
  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", tz, 1);
  tzset();
  delay(2000);
}
