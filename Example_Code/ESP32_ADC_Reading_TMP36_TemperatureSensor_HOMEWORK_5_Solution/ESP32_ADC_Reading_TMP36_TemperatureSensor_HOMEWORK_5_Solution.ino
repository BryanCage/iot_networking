/*
  TMP36 - Three Pins (Legs)
  Align with flat side facing you
    Left Pin            => 3.3v
    Middle Pin (Data)   => GPIO36 (VP)
    Right Pin           => GND
*/

#include <driver/adc.h>
#include "functions.h"

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer("test.mosquitto.org", 1883);
  client.setCallback(callback);
}

void loop() {

  /* Code in Main Loop Runs Repeatedly */
  if(!client.connected()) {
    reconnect();
  }
  
  getTemp();

}
