#include <WiFi.h>
#include "credentials.h"

void setup_wifi() {

  const char* ssid     = mySSID;
  const char* password = myPASSWORD;

  delay(10);
  /* We start by connecting to a WiFi network */
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(mySSID, myPASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
