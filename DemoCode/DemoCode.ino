#include <HardwareSerial.h>
#include <Wire.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


#include <secrets.h>

AsyncWebServer server(80);


struct GPS_Position {
  double latitude = 46.205385849685236;
  double longitude = -63.75803622670995;
  double Altitude;
};

GPS_Position Current_Pos;

void setup() {
  Serial.begin(115200);
  Wire.begin();


  GPS_StartUp(4, 5);
  LCD_Start();
  MPU_Start();
  MAX30100_Start();

  WebServer_Start();

  LCD_WriteString("Speed:\nCadence:");
}

void loop() {
  static char NMEA_Sentance[80];
  static uint16_t Current_Time; 

  //If a new sentance is returned;
  if (GPS_ReadSentance(NMEA_Sentance) == 0){
    //Serial.print(NMEA_Sentance);
    
    if (GPS_ParseSentance(NMEA_Sentance) == 1){
      GPS_CaluclateData(&Current_Pos, &Current_Time);
    }

  }


  
  uint16_t data;
  if(MAX30100_ReadFIFO(&data)){
    float dcremoved = RemoveDc(data);

    float median = 0;
    static float last_data = 0;
    if(Median_Filter(dcremoved, &median)){
      float BPM;
      if(MAX30100_PeakDectection(last_data - median, &BPM)){
        Serial.println(BPM);
      }
      last_data = median;
    }
  }


  int16_t MPU6050_Data[7];
  MPU6050_BurstRead(MPU6050_Data);

}
