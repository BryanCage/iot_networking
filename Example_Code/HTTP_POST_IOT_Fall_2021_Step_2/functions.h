#include <driver/adc.h>
#include <WiFi.h>
#include <HTTPClient.h>

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

double getTemp() {

  double voltage;
  double tempC;
  double tempF;
  
  // Read Temperature from ADC
  adc1_config_width(ADC_WIDTH_BIT_10);
  adc1_config_channel_atten( ADC1_CHANNEL_0, ADC_ATTEN_0db );
  int raw_val = adc1_get_raw(ADC1_CHANNEL_0); // GPIO 36 - VP; On some boards it is screen printed SVP
  
  voltage = raw_val/1024.0;

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
  
  return tempF;
    
}

void sendPost(int device_id) {

    //Your Domain name with URL path or IP address with path
    const char* serverName = "https://iotnetworking.iotserver.website/cage/insert_temp_record.php";
    
    // Reconnect to WiFi in not connected
    if(WiFi.status() != WL_CONNECTED) {
      setup_wifi();
    }
    if(WiFi.status()== WL_CONNECTED){
      
      double tempF = getTemp();
      
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      // Data to send with HTTP POST
      String httpRequestData = "device_id="+ String(device_id) + "&temp=" + String(tempF);           
      
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
 
      String httpResponseText = "";
      httpResponseText = http.getString();
      Serial.println(httpResponseText);
 
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}
