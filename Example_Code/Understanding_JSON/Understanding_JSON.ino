#include <Arduino_JSON.h>
#include <time.h>

// Declare String Variable to hold stringified version of JSON Object
String stringJSON;

// Declare and Initialize JSON Object
JSONVar jsonObject;

void setup() {
 Serial.begin(115200);

 // Create a latitude and longitude stored as doubles
 double flat = 38.12345;  // 8 characters long
 double flon = -78.12345; // 9 characters long

 // Create char arrays to hold converted doubles
 char latitude[9]; 
 char longitude[10];

 // Convert doubles to strings
 sprintf(latitude, "%02.5f", flat);
 sprintf(longitude, "%02.5f", flon);

 // Capture the values into their respective keys
 jsonObject["latitude"] = latitude;
 jsonObject["longitude"] = longitude;
 jsonObject["IoT_DeviceID"] = 1;

 // The stringified verison of JSON data can be sent in the payload of MQTT Messages
 stringJSON = JSON.stringify(jsonObject);
 // Print out the stringified version of the JSON data
 Serial.println("JSON String: ");Serial.println(JSON.stringify(jsonObject));
 
 // Demonstrate how to parse the JSON string back into a JSON Object
 JSONVar newJsonObject = JSON.parse(stringJSON);
 Serial.println(newJsonObject["latitude"]);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("JSON String: ");Serial.println(stringJSON);
  Serial.print("JSON Object: ");Serial.println(jsonObject);
  delay(3000);
}
