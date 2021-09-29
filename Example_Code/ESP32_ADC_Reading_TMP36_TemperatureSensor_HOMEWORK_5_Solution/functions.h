/* Define the Topic for posting temperature */
#define MQTT_TEMP_PUBLISH_CH "iotnetworking/temp/your_last_name/tx"

#include "credentials.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino_JSON.h>

float voltage;
double tempC;
double tempF;
char chr_tempF[15];
float aref = 3.3;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

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

void reconnect() {
  
  /* Loop until we're reconnected */
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    /* Create a random client ID */
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    /* Attempt to connect */
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      /* Once connected, publish the successful connection to the topic below */
      client.publish("mqttPublish/connectionStatus/","ESP32 Connection Successful.");
      
      /* subscribe to the topic mqttSubscribe/ */
      client.subscribe("mqttSubscribe/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      /* Wait 1 second and attempt connection again */
      delay(1000);
    }
  }
}

/* Publish temperatur reading to topic #defined in line */
void publishTemp(char *p_temp) {
  
  if(!client.connected()) {
    reconnect();  
  }
  client.publish(MQTT_TEMP_PUBLISH_CH, p_temp);
}

// Custom Function
void getTemp() {

  // Read Temperature from ADC
  adc1_config_width(ADC_WIDTH_BIT_10);
  adc1_config_channel_atten( ADC1_CHANNEL_0, ADC_ATTEN_0db );
  int raw_val = adc1_get_raw(ADC1_CHANNEL_0); // GPIO 36 - VP; On some boards it is screen printed SVP
  
  voltage = (raw_val/1024.0);

  // Convert voltage reading to temperature in °C
  tempC = (voltage - 0.5) * 100; // Use with ESP32
  
  // Convert °C to °F
  tempF = (tempC * 9.0 / 5.0) + 32.0;

  // Print to Serial Monitor for Debugging
  Serial.print("Temp F: ");
  Serial.println(tempF);
  Serial.print("Raw Reading: ");Serial.println(raw_val);
  Serial.print("Voltage: ");Serial.println(voltage);
  Serial.println();

  /* Copy tempF (float) into string (chr_tempF) formatted as JSON 
   * {"temp":"65.5"}
   */
  sprintf(chr_tempF, "{\"temp\":\"%02.1f\"}", tempF);
   
  /* Publish JSON payload in MQTT packet using publishTemp() function */
  publishTemp(chr_tempF);
  
  /* Print Temperature every 5 seconds to Serial Monitor */
  delay(5000);
}

/* This function is called everytime this ESP32 receives a new MQTT Message */
void callback(char* topic, byte* payload, unsigned int length) {
  
  /* Print Topic to Serial Monitor */
  Serial.print("Message arrived [Topic: ");
  Serial.print(topic);
  Serial.print(" Payload: ");
  
  /* Loop through payload array and cast the integers into their characters and
     print characters to serial monitor */
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("]");Serial.println();

} 
