#define WheelSwitch 26
#define PedalSwitch 25


void IRAM_ATTR Reed_Wheel(){
  static uint32_t PREV_WHEEL_TIMESTAMP = 0;
  uint32_t Currnet_Time = millis();

  if(PREV_WHEEL_TIMESTAMP - Currnet_Time > 1){
    WHEEL_RPM_COUNT++;
  }
  PREV_WHEEL_TIMESTAMP = millis();
}

void ReedSwitchSetup(){
  attachInterrupt(digitalPinToInterrupt(WheelSwitch), Reed_Wheel, RISING);
}
