#define MQTT_TEMP_PUBLISH_CH "iotnetworking/node1/temp/tx"
#define MQTT_SERIAL_RECEIVER_CH "iotnetworking/node1/rx"

#include "credentials.h"
#include <driver/adc.h>
#include <WiFi.h>
#include <WiFiClientSecure.h> /* WiFiClient Library for encrypting data over SSL/TSL */
#include <PubSubClient.h>
#include <Arduino_JSON.h>

float voltage;
double tempC;
double tempF;
char chr_tempF[30];
float aref = 3.3;

#include "certificate.h"
WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

/* ===== ADD TIMER INTERRUPTS ========================= */
volatile int interrupts;
int counter = 1;

/* Instantiate New Timer Interrupt and Enable */
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
/* ===================================================== */

void connectMQTTBroker() {
  
  /* Loop until we're reconnected */
  unsigned long startAttemptConnectMQTTTime = millis();
  while (!client.connected() and startAttemptConnectMQTTTime - millis() < 5000) {
    Serial.print("Attempting MQTT connection...");
    
    /* Create a random client ID */
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    /* Attempt to connect */
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      
      /* Once connected, publish an announcement... */
      client.publish("iotnetwork/connStat/", "Bryan's ESP 32 Connected");
      
      /* ... and resubscribe */
      client.subscribe(MQTT_SERIAL_RECEIVER_CH);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}

void handle_light(byte *payload, unsigned int length) {

  /* Reconnect to MQTT Broker if disconnected */
  if (!client.connected()) {
    connectMQTTBroker();
  }

  /*  Declare character array equal to the size of incoming payload plus 1.
      Character arrays (strings) must have an additional byte to hold the
      NULL termination character of '\0'. NULL termination character marks the
      end of the string.
  */
  char charArray[length + 1];

  /* memcpy => Copy block of memory
     Copies the values of num bytes from the location pointed to by source directly
     to the memory block pointed to by destination.

     void * memcpy ( void * destination, const void * source, size_t num );
  */
  memcpy(charArray, payload, length);

  /* Set the last item in the array as the NULL termination character
     to mark the end of the string.
  */
  charArray[length] = NULL;

  /* Cast the charArray into a Arduino String Object */
  String message(charArray);
  //  Serial.println(message);

  /* Use JSON.parse() function to parse string into a JSONVar Object */
  JSONVar jsonObject = JSON.parse(message);

 
  /* Capture the value of device id */
  int deviceID = (int) jsonObject["deviceID"];
   /* Capture the value of jsonObject["message"] into a boolean variable */
  bool lightStatus = jsonObject["message"];
   
  if (device_id = deviceID) {
    
    /* Print values of each Object key to the serial monitor */
    Serial.println();
    Serial.println("Device ID Match: Parsing values from keys in JSON Object: " );
     /* Print message to indicate reception from a particular device */
    Serial.print("Device ID: ");Serial.print(deviceID);
    Serial.print(" | Light Status: ");Serial.println(lightStatus);
    Serial.println();
    
    /* Compare the boolean values in order to set LED Pin5 HIGH or LOW */
    if (lightStatus == true) {
      Serial.println("Turning the light on.");
      Serial.println("========================================================");
      Serial.println();
      /* Turn Pin 5 LED On */
      digitalWrite(5, HIGH);
    } else if (lightStatus == false) {
      Serial.println("Turning the light off.");
      Serial.println("========================================================");
      Serial.println();
      /* Turn Pin 5 LED On */
      digitalWrite(5, LOW);
    }
  }
}

/* This function is called everytime this ESP32 receives a new MQTT Message */
void callback(char* topic, byte* payload, unsigned int length) {
  
  /* Print Topic to Serial Monitor */
  Serial.println("========================================================");
  Serial.println("Message arrived");
  Serial.println();
  Serial.print("   Topic: ");
  Serial.println(topic);
  Serial.print("   Payload: ");
  
  /* Loop through payload array and cast the integers into their characters and
     print characters to serial monitor */
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  handle_light(payload, length);
}

void setup_wifi() {

    // Update these with values suitable for your network.
    const char* ssid = mySSID;          /* Stored in credentials.h */
    const char* password = myPASSWORD;  /* Stored in credentials.h */
    const char* MQTT_SERVER = "bcmediamqttserver.com";  /* Name of the Server running the Mosquitto MQTT Broker */
    
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    /* Set the CA_CERTIFICATE to be used by wifiClient -- root_ca must be created
       copied, and pasted above -- Tutorial to create ca_root certificate: https://www.youtube.com/watch?v=Wm1xKj4bKsY */   
    wifiClient.setCACert(root_ca);

    Serial.println("\nStarting connection to server...");
    if(!wifiClient.connect(MQTT_SERVER, 443)) {
      Serial.println("Connection Failed!");
    } else {
      Serial.println("Connected to server!");
      wifiClient.println("GET / HTTP/1.0");
      wifiClient.println("Host: www.bcmediamqttserver.com");
      wifiClient.println("Connection: close");
      wifiClient.println();

      while(wifiClient.connected()) {
        String line = wifiClient.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("headers received");
          break;
        }
      }
      while (wifiClient.available()) {
        char c = wifiClient.read();
        Serial.write(c);
      }
      wifiClient.stop();
    }
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
    connectMQTTBroker();
}

void publishTemp(char *p_temp) {
  
  if(!client.connected()) {
    connectMQTTBroker();  
  }
  client.publish(MQTT_TEMP_PUBLISH_CH, p_temp);
}

void getTemp() {

  /* Read Temperature from ADC */
  adc1_config_width(ADC_WIDTH_BIT_10);
  adc1_config_channel_atten( ADC1_CHANNEL_0, ADC_ATTEN_0db );
  int raw_val = adc1_get_raw(ADC1_CHANNEL_0); // GPIO 36 - VP; On some boards it is screen printed SVP
  
  voltage = (raw_val/1024.0);

  /* Convert voltage reading to temperature in °C */
  tempC = (voltage - 0.5) * 100; // Use with ESP32
  
  /* Convert °C to °F */
  tempF = (tempC * 9.0 / 5.0) + 32.0;

  /* Print to Serial Monitor for Debugging */
  Serial.print("Temp F: ");
  Serial.println(tempF);
  Serial.print("Raw Reading: ");Serial.println(raw_val);
  Serial.print("Voltage: ");Serial.println(voltage);
  Serial.println("========================================================");
  Serial.println();

  /* Copy tempF into a JSON formatted string and store in chr_tempF */
  sprintf(chr_tempF, "{\"device_id\": %d, \"temp\":\"%02.1f\"}", device_id, tempF);
  
  /* Publish JSON String as MQTT Packet Payload */
  publishTemp(chr_tempF);
}

/* ISR for ESP Interrupt */
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interrupts++;
  portEXIT_CRITICAL_ISR(&timerMux);
}
