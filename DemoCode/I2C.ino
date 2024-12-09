void I2C_Write(uint8_t DeviceAddress, uint8_t RegAddress, uint8_t ToWrite){
  Wire.beginTransmission(DeviceAddress);
  Wire.write(RegAddress);
  Wire.write(ToWrite);
  Wire.endTransmission(true);
}

uint8_t I2C_Read(uint8_t DeviceAddress, uint8_t RegAddress){
  Wire.beginTransmission(DeviceAddress);
  Wire.write(RegAddress);
  Wire.endTransmission(false);

  Wire.requestFrom(DeviceAddress, 1);

  return Wire.read();
}



