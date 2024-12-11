#define I2C_HeartAddress 0x57

#define INT_STATUS 0x00
#define INT_ENABLE 0x01
#define WRITE_POINTER 0x02
#define READ_POINTER 0x04
#define FIFO 0x05
#define MODE_CONFIG 0x06
#define SP02_CONFIG 0x07
#define LED_CONFIG 0x09






void MAX30100_Start(){
  //Reset Device
  I2C_Write(I2C_HeartAddress, MODE_CONFIG, 0x40);
  delay(10);

  //Setup Device
  I2C_Write(I2C_HeartAddress, INT_ENABLE, 0x20);
  I2C_Write(I2C_HeartAddress, SP02_CONFIG, 0x43);
  I2C_Write(I2C_HeartAddress, MODE_CONFIG, 0x02);
  I2C_Write(I2C_HeartAddress, LED_CONFIG, 0xFF);
}

float RemoveDc(uint16_t Value){
  static float prev_value = 0;
  float intermediate = Value + (0.95 * prev_value);
  float result = intermediate - prev_value;
  prev_value = intermediate;
  
  return result;
}

uint8_t Median_Filter(float data, float* result){
  static float Data_Buffer[5];
  static uint8_t index = 0;

  Data_Buffer[index] = data;
  index++;

  if(index == 5){
    while(1){
      uint8_t Sorted = 1;
      for(uint8_t i = 0; i != 4; i++){
        if(Data_Buffer[i] > Data_Buffer[i + 1]){
          float temp = Data_Buffer[i];
          Data_Buffer[i] = Data_Buffer[i + 1];
          Data_Buffer[i + 1] = Data_Buffer[i];
          Sorted = 0;
        }
      }

      if(Sorted == 1){
        break;
      }

    }

    *result = Data_Buffer[2];
    index = 0;

    return 1;
  }

  return 0;

}

uint8_t MAX30100_PeakDectection(float data, float* result){
  static uint8_t State = 3;
  static float LastData = 0;

  static int64_t TimeStamp;
  static int64_t LastTimeStamp = 0;

  switch (State){
    case 0:
      if(data < 60){
        LastData = data;
        return 0;
      }
      State = 1;
      LastData = data;
      return 0;
    case 1:
      if (data > LastData){
        LastData = data;
        return 0;
      }
      TimeStamp = esp_timer_get_time();
      State = 2;
    case 2:
      *result = 60000000/ (float)(TimeStamp - LastTimeStamp);
      LastTimeStamp = TimeStamp;
      State = 3;
      return 1;
    case 3:
      if (data >= 50){
        State = 3;
        return 0;
      }
      State = 0;
      return 0;
  }
}


uint8_t MAX30100_ReadFIFO(uint16_t *IR_Data){
  if(I2C_Read(I2C_HeartAddress, INT_STATUS) == 0x00){
    return 0;
  }

  Wire.beginTransmission(I2C_HeartAddress);
  Wire.write(FIFO);
  Wire.endTransmission(false);


  Wire.requestFrom(I2C_HeartAddress, 4);

  uint16_t Temp = 0;

  Temp |= Wire.read() << 8;
  Temp |= Wire.read();

  *IR_Data = Temp;

  Wire.read();
  Wire.read();

  return 1;
}
