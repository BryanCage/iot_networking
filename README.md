# IoT Networking

Homework:
1. Apply for a student license for JetBrains Software: https://www.jetbrains.com/shop/eform/students
2. Download and Install PhpStorm IDE: https://www.jetbrains.com/phpstorm/download/#section=windows
3. Download and Install MQTT.fx software: http://www.jensd.de/apps/mqttfx/1.7.1/</br></br>
   a. mqttfx-1.7.1-macos.dmg  2018-09-28 14:27   55M  (**Mac Users**)</br>
   b. mqttfx-1.7.1-windows..> 2018-09-28 14:27   51M  (**Windows Users**)</br>

We will be using PhpStorm Integrated Development Environment for creating and working with our server files and directories. Please apply for a **FREE** Professional version of PhpStorm, download it and install onto your laptop.

We will be using MQTT.fx software to **Subscribe** and **Publish** MQTT Packets for monitoring and testing.

### ESP32 Wiring for TMP36 Temperature Sensor (GPIO36 - VP or SVP) and LED (GPIO5)

1. Two Breadboards
2. One 220 Ohm resistor
3. ESP32 Dev Board
4. One TMP36 Temperature Sensor
5. One Light Emitting Diode (LED) of your color choice

![Wiring](images/IotNetworkingWiring_GPIO36_for_TMP.png)

<img src="images/ESP32_WROOM_32D_Pinouts.svg"/>

### Monitoring Temperature with the TMP36
```c
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
  adc1_config_channel_atten( ADC1_CHANNEL_4, ADC_ATTEN_0db );
  int raw_val = adc1_get_raw(ADC1_CHANNEL_4); // GPIO 32
  
  voltage = raw_val/4096.0;

  // Convert voltage reading to temperature in °C
  tempC = (voltage - 0.5) * 100; // Use with ESP32
  
  // Convert °C to °F
  tempF = (tempC * 9.0 / 5.0) + 32.0;

  // Print to Serial Monitor for Debugging
  Serial.print("Temp C: ");
  Serial.println(tempC);
  Serial.print("Raw Reading: ");Serial.println(raw_val);
  Serial.print("Voltage: ");Serial.println(voltage);

  // Print Temperature every 5 seconds to Serial Monitor
  delay(5000);
}
```


