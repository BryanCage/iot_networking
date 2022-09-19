#include "credentials.h"
#include "functions.h"

const char* ssid = mySSID;
const char* password = myPASSWORD;

// Your Device's Id
int device_id = 1;

void setup() {
  Serial.begin(115200);

  /* Set up wifi connection */
  setup_wifi();
  sendPost(device_id);
}

void loop() {

  //Send an HTTP POST request every 5 seconds
 
}
