#include <HardwareSerial.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "FS.h"

#include <secrets.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
HardwareSerial GPS_Serial(1);


struct GPS_Position {
  uint8_t fix = 0;
  double latitude = 46.205385849685236;
  double longitude = -63.75803622670995;
  double altitude = 0;
};

GPS_Position Current_Pos;

volatile uint32_t WHEEL_RPM_COUNT = 0;
volatile uint32_t PEDAL_RPM_COUNT = 0;

volatile uint8_t HR_DATA_READY = 0;

float wheelDia = 0.1; //Wheel diamater in meters
float circufrance = wheelDia * 3.14159;


void setup() {
  Serial.begin(115200);
  Wire.begin();

  GPS_StartUp(5, 4);
  LCD_Start();
  MPU_Start();
  MAX30100_Start();
  ReedSwitchSetup();

  if(!LittleFS.begin()){
    LCD_WriteString("Error!\n No file system!");
  }

  WebServer_Start();
  
  LCD_Clear();
  LCD_WriteString("Speed: 0 Km/h\nCadence: 0 RPM");
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
  
  uint16_t data = 0;
  static uint16_t old_data = 0;
  if(MAX30100_ReadFIFO(&data)){
    float DC_Removed = RemoveDc(data);
    //Serial.println(MAX30100_MovingAverage(DC_Removed));
  }

  int16_t MPU6050_Data[7];
  MPU6050_BurstRead(MPU6050_Data);

  static uint32_t Last_Calculation = 0;
  static uint32_t Wheel_RPM = 0;
  static uint32_t Pedal_RPM = 0;
  if (millis() - Last_Calculation >= 1000){
    uint32_t Current_Time = millis();
    Wheel_RPM = (WHEEL_RPM_COUNT/((Current_Time - Last_Calculation) / 1000.0)) * 30;
    WHEEL_RPM_COUNT = 0;

    Pedal_RPM = (PEDAL_RPM_COUNT/((Current_Time - Last_Calculation) / 1000.0)) * 30;
    PEDAL_RPM_COUNT = 0;

    Last_Calculation = Current_Time;
    
    char buffer[50];
    LCD_Clear();
    sprintf(buffer, "Speed: %.1fKm/H\nCadance %lu RPM", (((float)Wheel_RPM * circufrance) * 3.6), Pedal_RPM);
    LCD_WriteString(buffer);

  }

  
  static uint32_t Last_Cleanup = 0;
  if (millis() - Last_Cleanup >= 1000){
    ws.cleanupClients();
    Last_Cleanup = millis();
  }

  
  static uint32_t Last_Data_Sent = 0;
  if (millis() - Last_Data_Sent >= 1000){
    StaticJsonDocument<200> json_doc;
    char serial_json[200];

    if(Current_Pos.fix != 0){
      json_doc["fix"] = 1;
      json_doc["latitude"] = Current_Pos.latitude;
      json_doc["longitude"] = Current_Pos.longitude;
      json_doc["altitude"] = Current_Pos.altitude;
    }else{
      json_doc["fix"] = 0;
    }

    size_t lenght = serializeJson(json_doc, serial_json);
    ws.textAll(serial_json, lenght);

    Last_Data_Sent = millis();
  }
}






