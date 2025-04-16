#define I2C_HeartAddress 0x57

#define INT_STATUS 0x00
#define INT_ENABLE 0x01
#define WRITE_POINTER 0x02
#define READ_POINTER 0x04
#define FIFO 0x05
#define MODE_CONFIG 0x06
#define SP02_CONFIG 0x07
#define LED_CONFIG 0x09
 
#define HR_INT_PIN 19

void IRAM_ATTR HR_READY(){
  HR_DATA_READY = 1;
}

void MAX30100_Start(){
  //Reset Device
  I2C_Write(I2C_HeartAddress, MODE_CONFIG, 0x40);
  delay(10);

  //Setup Device
  I2C_Write(I2C_HeartAddress, INT_ENABLE, 0x20);
  I2C_Write(I2C_HeartAddress, SP02_CONFIG, 0x0C);
  I2C_Write(I2C_HeartAddress, MODE_CONFIG, 0x02);
  I2C_Write(I2C_HeartAddress, LED_CONFIG, 0xFF);

  //Setup Intrupt
  //attachInterrupt(digitalPinToInterrupt(HR_INT_PIN), HR_READY, FALLING);
}



float RemoveDc(uint16_t Value){
  static float prev_value = 0;
  float intermediate = (float)Value + (0.95 * prev_value);
  float result = intermediate - prev_value;
  prev_value = intermediate;
  
  return result;
}

float MAX30100_MovingAverage(float data){
  static float window[15];
  static float sum = 0;
  static uint8_t index = 0;
  
  sum -= window[index];
  window[index] = data;
  sum += data;
  index = (index + 1) % 15;
  
  return sum/15;
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

  Temp >>= 3;
  *IR_Data = Temp;

  Wire.read();
  Wire.read();

  return 1;
}
