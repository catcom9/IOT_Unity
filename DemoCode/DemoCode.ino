#include <HardwareSerial.h>
#include <Wire.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>



//#include <secrets.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
HardwareSerial GPS_Serial(1);


struct GPS_Position {
  double latitude = 46.205385849685236;
  double longitude = -63.75803622670995;
  double Altitude;
};

GPS_Position Current_Pos;

volatile uint32_t WHEEL_RPM_COUNT = 0;


void setup() {
  Serial.begin(115200);
  Wire.begin();


  GPS_StartUp(5, 4);
  LCD_Start();
  MPU_Start();
  MAX30100_Start();
  ReedSwitchSetup();

  //WebServer_Start();

  LCD_WriteString("Speed:\nCadence:");
}

void loop() {

  static char NMEA_Sentance[80];

  //If a new sentance is returned;
  if (GPS_ReadSentance(NMEA_Sentance)){
    //Serial.print(NMEA_Sentance);
    if (GPS_ParseSentance(NMEA_Sentance) == 1){
      GPS_CaluclateData(&Current_Pos);
    }
  }

  
  uint16_t data;
  static uint16_t old_data = 0;
  if(MAX30100_ReadFIFO(&data)){
    Serial.println(data - old_data);
    old_data = data;
  }

  int16_t MPU6050_Data[7];
  MPU6050_BurstRead(MPU6050_Data);

  static uint32_t Last_Calculation = 0;
  static float Wheel_RPM = 0;
  if (millis() - Last_Calculation >= 1000){
    uint32_t Current_Time = millis();
    Wheel_RPM = ((float)WHEEL_RPM_COUNT/(float)((Current_Time - Last_Calculation) / 1000.0)) * 30;
    WHEEL_RPM_COUNT = 0;
    Last_Calculation = Current_Time;
    //Serial.println(Wheel_RPM);
  }


}
