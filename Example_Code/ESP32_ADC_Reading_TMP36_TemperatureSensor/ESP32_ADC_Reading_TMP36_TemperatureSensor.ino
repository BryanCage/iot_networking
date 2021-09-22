/*
  TMP36 - Three Pins (Legs)
  Align with flat side facing you
    Left Pin            => 3.3v
    Middle Pin (Data)   => GPIO36 (VP)
    Right Pin           => GND
*/

#include <driver/adc.h>

float voltage;
double tempC;
double tempF;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  getTemp();
}

// Custom Function
void getTemp() {

  // Read Temperature from ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten( ADC1_CHANNEL_0, ADC_ATTEN_0db );
  int raw_val = adc1_get_raw(ADC1_CHANNEL_0); // GPIO 36 - VP; On some boards it is screen printed SVP
  
  voltage = raw_val/4096.0;

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
  
  // Print Temperature every 5 seconds to Serial Monitor
  delay(5000);
}
