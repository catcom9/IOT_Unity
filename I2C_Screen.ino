#define ScreenAddress 0x3E

void LCD_Start(){
  I2C_Write(ScreenAddress, 0x80, 0x28);
  delay(10);
  I2C_Write(ScreenAddress, 0x80, 0x0F);
  delay(10);
  I2C_Write(ScreenAddress, 0x80, 0x01);
  delay(10);
  I2C_Write(ScreenAddress, 0x80, 0x06);
}



//C type null char string
void LCD_WriteString(char ToSend[]){
  uint8_t i = 0;
  while(ToSend[i] != '\0'){
    if(ToSend[i] == '\n'){
      I2C_Write(ScreenAddress, 0x80, 0xC0);
    } else{
      I2C_Write(ScreenAddress, 0xC0, ToSend[i]);
    }
    i++;
  }
}

void LCD_WriteAC(uint8_t Address){
  I2C_Write(ScreenAddress, 0x80, 0x80 | Address);
}








