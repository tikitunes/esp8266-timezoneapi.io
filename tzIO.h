#pragma once
#include <ESP8266WiFi.h>
#include "JsonStreamingParser.h"
#include "JsonListener.h"

class TimezoneAPI:public JsonListener {
private:
  void grabData();
  String currentKey = "";
  String currentParent = "";
  String offsetHours;
  String tzinfo;
public:
  TimezoneAPI();
  void getData();
  String getOffset();
  String getTZinfo();
  virtual void whitespace(char c);
  virtual void startDocument();
  virtual void key(String key);
  virtual void value(String value);
  virtual void endArray();
  virtual void endObject();
  virtual void endDocument();
  virtual void startArray();
  virtual void startObject();
};

TimezoneAPI::TimezoneAPI() {}
void TimezoneAPI::getData() {
  grabData();
}

void TimezoneAPI::grabData() {
  JsonStreamingParser parser;
  WiFiClient client;
  const char *host = "timezoneapi.io";
  String url = "/api/ip";
  const int port = 80;
  parser.setListener(this);
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "User-Agent: UNK\r\n" + 
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  char c;
  int size = 0;
  bool isBody = false;
  client.setNoDelay(false);
  while (client.connected()) {
    while ((size = client.available()) > 0) {
      c = client.read();
      if (c == '{' || c == '[') {
        isBody = true;
      }
      if (isBody) {
        parser.parse(c);
      }
    }
  }
}
void TimezoneAPI::startDocument() {
  currentKey = "";
  currentParent = "";
}
void TimezoneAPI::whitespace(char c) {
}
void TimezoneAPI::startArray() {
}
void TimezoneAPI::endArray() {
}
void TimezoneAPI::startObject() {
  currentParent = currentKey;
}
void TimezoneAPI::endObject() {
  currentParent = "";
}
void TimezoneAPI::endDocument() {
}
void TimezoneAPI::key(String key) {
  currentKey = String(key);
}
void TimezoneAPI::value(String value) {
  if (currentParent == "datetime" && currentKey == "tz_string") {
    tzinfo = value;
  }
  if (currentParent == "datetime" && currentKey == "offset_hours") {
    offsetHours = value;
  }
}
String TimezoneAPI::getOffset() {
  return offsetHours;
}
String TimezoneAPI::getTZinfo() {
  return tzinfo;
}

