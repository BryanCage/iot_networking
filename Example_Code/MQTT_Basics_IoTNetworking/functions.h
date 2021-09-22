
#include "credentials.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino_JSON.h>

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
      /* Once connected, publish the successful connection topic below */
      client.publish("mqttPublish/connectionStatus/","ESP32 Connection Successful.");
      /* subscribe to mqttSubscribe/ */
      client.subscribe("mqttSubscribe/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(1000);
    }
  }
}

void handle_light(byte *payload, unsigned int length) {

  /* Reconnect to MQTT Broker if disconnected */
  if(!client.connected()) {
    reconnect();
  }

  /*  Declare character array equal to the size of incoming payload plus 1.
   *  Character arrays (strings) must have an additional byte to hold the 
   *  NULL termination character of '\0'. NULL termination character marks the 
   *  end of the string.
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

  /* Print values of each Object key to the serial monitor */
  Serial.println("Parsing values from keys in JSON Object: " );
  Serial.print("jsonObject[\"lastName\"]: ");Serial.println(jsonObject["lastName"]);
  Serial.print("jsonObject[\"message\"]: ");Serial.println(jsonObject["message"]);

  /* Capture the value of jsonObject["message"] into a boolean variable */
  bool lightStatus = jsonObject["message"];
  
  /* Compare the boolean values in order to set LED Pin5 HIGH or LOW */
  if(lightStatus == true){
    Serial.println("Turning the light on!");
    /* Turn Pin 5 LED On */
    digitalWrite(5, HIGH);
  } else if(lightStatus == false){
    Serial.println("Turning the light off");
    /* Turn Pin 5 LED On */
    digitalWrite(5, LOW);
  } else {
    /* Print warning that the MQTT packet did not contain light data */
    Serial.println("The message did not contain light data!");
  }
}

/* This function is called everytime this ESP32 receives a new MQTT Message */
void callback(char* topic, byte* payload, unsigned int length) {
  
  /* Loop through payload array and print as binary representation of variable 
     stored in memory */
//  for(int i = 0; i<length; i++) {
//    Serial.print(payload[i],BIN);Serial.print(",");
//  }
//  Serial.println();
  
  /* Loop through payload array and display bytes as integer 
     representations of characters */
//  for(int i = 0; i<length; i++) {
//    Serial.print(payload[i]);Serial.print(",");
//  }
//  Serial.println();

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

  
  /* Custom function to turn the light on/off */
  handle_light(payload, length);
}
