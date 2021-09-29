/* for interrupts -- Tutorial for Using Timer Interrupts with ESP32
   https://www.visualmicro.com/page/Timer-Interrupts-Explained.aspx
   Variabes necessary for using ESP32 timer interrupt 
   (i.e. send temp updates)

   ESP32 Core Library: https://github.com/espressif/arduino-esp32/blob/af35773d65a8f328dba8090dff66ba43abe0e7be/cores/esp32/esp32-hal-timer.c#L216-L265
*/
volatile int interrupts;
int counter = 1;

/* Handle Interrupts */
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

/* ISR for ESP Interrupt */
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interrupts++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {

  Serial.begin(115200);

  /* Configure Prescaler to 80, as our timer runs @ 80Mhz
     Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second */
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  /* Fires Interrupt Service Routine (ISR) 
     every 5,000,000/1,000,000 = 5 seconds;
     The ISR is defined on lines 14-17 called onTimer() */
  timerAlarmWrite(timer, 5000000, true);
  timerAlarmEnable(timer);
}

void loop() {

  if (interrupts > 0) {
    portENTER_CRITICAL(&timerMux);
    interrupts--;
    portEXIT_CRITICAL(&timerMux);
    Serial.println();
    Serial.println("=======================");
    Serial.println("Interrupt Triggered");
    Serial.println("=======================");
    Serial.println();
    counter = 1;
  }
  Serial.print("Executing Main Code: ");Serial.println(counter);
  counter +=1;
  delay(1000);

}
