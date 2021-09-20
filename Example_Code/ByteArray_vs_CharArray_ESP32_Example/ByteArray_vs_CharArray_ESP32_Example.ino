void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  byte byteArray[] = { 'a', 'b', 'c'};
  char charArray[] = { 'a', 'b', 'c'};

  for(int i = 0; i<3; i++) {
    if(i < 2) {
      Serial.print(byteArray[i]);Serial.print(",");
    } else {
      Serial.print(byteArray[i]);
    }
  }
  Serial.println();
  for(int i = 0; i<3; i++) {
    if(i < 2) {
      Serial.print(charArray[i]);Serial.print(",");
    } else {
      Serial.print(charArray[i]);
    }
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
