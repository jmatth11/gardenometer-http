//Drivers for ESP8266
//https://sparks.gogo.co.nz/assets/_site_/downloads/CH34x_Install_Windows_v3_4.zip

#include "scan.h"
#include "state.h"
#include <ESP8266HTTPClient.h>
#include <WString.h>

const char *ssid = "";
const char *password = "";
const char *path = "/status";
const char *key = "gardenometer";
const int port = 8000;

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
}

int parse_serial(String data) {
  // TODO implement parsing
  return 1;
}

String parse_response(String data) {
  // TODO implement parsing
  return "";
}

void loop() {
  String payload;
  if (Serial.available()) {
    payload = Serial.readStringUntil('\n');
    if (parse_serial(payload)) {
      state.data = payload;
      state_machine.state = SENDING_DATA;
    }
  }
  handle_state_machine(&state_machine, &state);
  delay(1000);
}

void garden_no_connection(state_machine_t *sm, void *context) {
  struct garden_state *state = (struct garden_state *)context;
  HTTPClient http;
  if (scan_ips_for_website(port, path, key, &http)) {
    sm->state = NONE;
    state->http = http;
  }
}

void garden_sending_data(state_machine_t *sm, void *context) {
  struct garden_state *state = (struct garden_state *)context;
  int httpCode = state->http.POST(state->data.c_str(), state->data.length());
  if (httpCode == HTTP_CODE_OK) {
    String response = parse_response(state->http.getString());
    if (response != "") {
      Serial.println("res: " + body);
    }
    sm->state = NONE;
  } else if (httpCode < 0 || httpCode > 299) {
    sm->state = ERROR;
    state->data = state->http.getString();
  }
}

void garden_error(state_machine_t *sm, void *context) {
  struct garden_state *state = (struct garden_state *)context;
  if (sm->state = ERROR) {
    Serial.println(ERROR);
    Serial.println(state->data);
  } else {
    Serial.println(CLEAR_ERROR);
  }
}
