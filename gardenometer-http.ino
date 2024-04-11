//Drivers for ESP8266
//https://sparks.gogo.co.nz/assets/_site_/downloads/CH34x_Install_Windows_v3_4.zip

#include "scan.h"
#include "state.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WString.h>

const char *ssid = "";
const char *password = "";
const char *path = "/status";
const char *key = "gardenometer";
const int port = 8000;
const char *status = "status:";

struct garden_state {
  HTTPClient http;
  String data;
};

state_machine_t state_machine;
struct garden_state state;

void garden_no_connection(state_machine_t *sm, void *context);
void garden_sending_data(state_machine_t *sm, void *context);
void garden_error(state_machine_t *sm, void *context);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // wait until connected
    delay(500);
  }
  state_machine.state = NO_CONNECTION;
  state_machine.no_connection = garden_no_connection;
  state_machine.sending_data = garden_sending_data;
  state_machine.error = garden_error;
  state.http.setReuse(true);
}

int parse_serial(String data) {
  if (data.startsWith(status)) {
    return 1;
  }
  return 0;
}

void loop() {
  state.data = "";
  if (Serial.available() && state_machine.state != NO_CONNECTION) {
    state.data = Serial.readStringUntil('\n');
    if (parse_serial(state.data)) {
      state_machine.state = SENDING_DATA;
    }
  }
  handle_state_machine(&state_machine, &state);
  delay(1000);
}

void garden_no_connection(state_machine_t *sm, void *context) {
  struct garden_state *state = (struct garden_state *)context;
  if (scan_ips_for_website(port, path, key, &state->http)) {
    sm->state = NONE;
  }
}

void garden_sending_data(state_machine_t *sm, void *context) {
  struct garden_state *state = (struct garden_state *)context;
  String s;
  int httpCode = state->http.POST((uint8_t*)state->data.c_str(), state->data.length());
  if (httpCode == HTTP_CODE_OK) {
    String response = state->http.getString();
    if (response != "") {
      // TODO maybe don't add "res:"
      Serial.println("res:" + response);
    }
    sm->state = NONE;
  } else if (httpCode == HTTPC_ERROR_CONNECTION_FAILED) {
    sm->state = NO_CONNECTION;
  } else if (httpCode < 0 || httpCode > 299) {
    sm->state = ERROR;
    state->data = state->http.getString();
  }
}

void garden_error(state_machine_t *sm, void *context) {
  struct garden_state *state = (struct garden_state *)context;
  if (sm->state == ERROR) {
    Serial.print("code:");
    Serial.println(ERROR);
    Serial.println(state->data);
  } else {
    Serial.print("code:");
    Serial.println(CLEAR_ERROR);
  }
}
