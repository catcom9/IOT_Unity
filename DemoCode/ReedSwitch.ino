#define WheelSwitch 26
#define PedalSwitch 25


void IRAM_ATTR Reed_Wheel(){
  static uint32_t PREV_WHEEL_TIMESTAMP = 0;
  uint32_t Currnet_Time = millis();

  if(PREV_WHEEL_TIMESTAMP - Currnet_Time > 2){
    PREV_WHEEL_TIMESTAMP = Currnet_Time;
    WHEEL_RPM_COUNT++;
  }
  PREV_WHEEL_TIMESTAMP = millis();
}

void IRAM_ATTR Reed_Pedal(){
  static uint32_t PREV_PEDAL_TIMESTAMP = 0;
  uint32_t Currnet_Time = millis();

  if(PREV_PEDAL_TIMESTAMP - Currnet_Time > 1){
    PREV_PEDAL_TIMESTAMP = Currnet_Time;
    PEDAL_RPM_COUNT++;
  }
  PREV_PEDAL_TIMESTAMP = millis();
}


void ReedSwitchSetup(){
  attachInterrupt(digitalPinToInterrupt(WheelSwitch), Reed_Wheel, RISING);
  attachInterrupt(digitalPinToInterrupt(PedalSwitch), Reed_Pedal, RISING);
}
