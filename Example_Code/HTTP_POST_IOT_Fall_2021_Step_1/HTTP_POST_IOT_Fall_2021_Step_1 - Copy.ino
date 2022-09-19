#include <HTTPClient.h>
#include "functions.h"

//Your Domain name with URL path or IP address with path
const char* serverName = "https://iotnetworking.iotserver.website/cage/api_endpoint.php";

void setup() {
  
  /* Set up serial connection at baud of 115200 */
  Serial.begin(115200);
  /* Set up wifi connection */
  setup_wifi();

  /* Instantiating a new http object */
  HTTPClient http;

  /* Your Domain name with URL path or IP address with path */
  http.begin(serverName);

  /* Specify content-type header */
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  /* Data to send with HTTP POST */
  String httpRequestData = "lastName=Cage&firstName=Bryan";

  /* Send HTTP POST request */
  int httpResponseCode = http.POST(httpRequestData);

  String httpResponseText = "";
  httpResponseText = http.getString();
  Serial.println(httpResponseText);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  /* Free resources */
  http.end();
}


void loop() {

}
