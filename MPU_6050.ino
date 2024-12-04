#define MPU_6050 0x68

#define PWR_MGMT_1 0x6B
#define USER_CTRL 0x6A
#define INT_ENABLE 0x38
#define INT_STATUS 0x3A

void MPU_Start(){
  I2C_Write(MPU_6050, PWR_MGMT_1, 0x01);
  delay(10);
  I2C_Write(MPU_6050, USER_CTRL, 0x00);
  delay(10);
  I2C_Write(MPU_6050, INT_ENABLE, 0x01);
  delay(10);
}


uint8_t MPU6050_BurstRead(int16_t Data[7]){
  uint8_t Int_Status = I2C_Read(MPU_6050, INT_STATUS);
  if((Int_Status & 0x01) == 0){
    return 0;
  }


  Wire.beginTransmission(MPU_6050);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_6050, 14);

  for(uint8_t i = 0; i != 7; i++){
    Data[i] = Wire.read() << 8;
    Data[i] |= Wire.read();
  }

  return 1;
}