#include "scan.h"
#include <ESP8266WiFi.h>

int scan_ips_for_website(int port, String path, String key, HTTPClient *out) {
  String prepend = "http://192.168.";
  String url = "";
  HTTPClient local;
  for (int i = 0; i < 1; ++i) {
    for (int j = 1; j < 256; ++j) {
      url = prepend + i + "." + j;
      WiFiClient client;
      if (local.begin(client, url, port, path)) {
        int httpCode = local.GET();
        if (httpCode == HTTP_CODE_OK) {
          String payload = local.getString();
          if (payload == key) {
            *out = local;
            return 1;
          }
        }
      }
    }
  }
  return 0;
}
