#include "scan.h"
#include <ESP8266WiFi.h>

int scan_ips_for_website(int port, String path, String key, HTTPClient *http) {
  String prepend = "http://192.168.";
  String url = "";
  for (int i = 0; i < 1; ++i) {
    for (int j = 1; j < 256; ++j) {
      url = prepend + i + "." + j;
      WiFiClient client;
      if (http->begin(client, url, port, path)) {
        int httpCode = http->GET();
        if (httpCode == HTTP_CODE_OK) {
          String payload = http->getString();
          if (payload == key) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}
