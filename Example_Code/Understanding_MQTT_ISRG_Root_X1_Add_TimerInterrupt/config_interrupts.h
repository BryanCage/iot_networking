/* Configure Prescaler to 80, as our timer runs @ 80Mhz
     Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second */
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  /* Fires Interrupt Service Routine (ISR) 
     every 20,000,000/1,000,000 = 20 seconds;
     The ISR is defined on lines 14-17 called onTimer() */
  timerAlarmWrite(timer, 20000000, true);
  timerAlarmEnable(timer);
