/*
 Basic MQTT example
*/

#include "functions.h"

void setup()
{
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  setup_wifi();
  client.setServer("test.mosquitto.org", 1883); /* https://https://test.mosquitto.org  Port 1883 : MQTT, unencrypted, unauthenticated */
  client.setCallback(callback);

}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
