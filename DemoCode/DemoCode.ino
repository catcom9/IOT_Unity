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
HardwareSerial GPS_Serial(1);

void setup() {
  Serial.begin(115200);
  Wire.begin();


  GPS_StartUp(5, 4);
  LCD_Start();
  MPU_Start();
  MAX30100_Start();

  WebServer_Start();

  LCD_WriteString("Speed:\nCadence:");
}

void loop() {
  static char NMEA_Sentance[80];
  static uint16_t Current_Time;
  static char mode = '0';

  if(Serial.available()){
    mode = Serial.read();
  }

  //If a new sentance is returned;
  if (GPS_ReadSentance(NMEA_Sentance)){
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
      if(mode == '2'){
        Serial.println(median - last_data);
      }
      last_data = median;
    }
  }

  int16_t MPU6050_Data[7];
  MPU6050_BurstRead(MPU6050_Data);



  if (mode == '0'){
    static int64_t last_time = 0;
    if(esp_timer_get_time() - last_time >= 50000){
      last_time = esp_timer_get_time();

      Serial.println("Accelerometer");

      Serial.print("\tX: ");
      Serial.println(MPU6050_Data[0] / 16384);

      Serial.print("\tY: ");
      Serial.println(MPU6050_Data[1] / 16384);

      Serial.print("\tZ: ");
      Serial.println(MPU6050_Data[2] / 16384);

      Serial.println("Gyroscope");

      Serial.print("\tX: ");
      Serial.println(MPU6050_Data[4] / 131);

      Serial.print("\tY: ");
      Serial.println(MPU6050_Data[5] / 131);
      
      Serial.print("\tZ: ");
      Serial.println(MPU6050_Data[6] / 131);

      Serial.print("Temp: ");
      Serial.println(MPU6050_Data[3]/340 + 36.53);
    }
  }

  if(mode == '1'){
    static int64_t last_time = 0;
    if(esp_timer_get_time() - last_time >= 500000){
      Serial.print(NMEA_Sentance);
      last_time = esp_timer_get_time();
    }
  }








}
