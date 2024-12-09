#define WheelGPIOMask 0x0000
#define PedalGPIOMask 0x0000

//0 Wheel Switch, 1 Pedal Switch
uint8_t Reed_CheckSwitch(uint8_t ToCheck){
  static uint8_t LastState[2] = {0, 0};
  uint32_t Masks[2] = {WheelGPIOMask, PedalGPIOMask};

  if(GPIO_IN_REG & Masks[ToCheck]){

    if(LastState[ToCheck] == 0){
      LastState[ToCheck] = 1;
      return 1;
    }
    return 0;

  } else {
    LastState[ToCheck] = 0;
    return 0;
  }
}


