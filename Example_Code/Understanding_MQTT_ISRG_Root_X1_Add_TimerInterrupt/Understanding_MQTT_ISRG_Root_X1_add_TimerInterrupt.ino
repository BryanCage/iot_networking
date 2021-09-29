#define WIFI_TIMEOUT_MS 200
#define MQTT_PORT 8883

int device_id = 1;
int LED_pin = 5;

#include "functions.h"

void setup() {
  Serial.begin(115200);

  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, LOW);

  /* Add interrupt configurations */
#include "config_interrupts.h"

  /* Connect to WiFi and to Server via SSL for encrypted connection */
  setup_wifi();

  /* Connect to MQTT Broker */
  connectMQTTBroker();
}

void loop() {

  /* Listen for incoming MQTT Packets */
  client.loop();

  if (interrupts > 0) {
    portENTER_CRITICAL(&timerMux);
    interrupts--;
    portEXIT_CRITICAL(&timerMux);

    Serial.println("========================================================");
    Serial.println("Interrupt Triggered");
    /* Read New Temperature Value */
    getTemp();
  }
}
