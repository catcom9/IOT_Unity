


char GGA_Time[11];
char GGA_Latitude[10];
char GGA_NS_Indicator[2];
char GGA_Longitude[11];
char GGA_EW_Indicator[2];
char GGA_Position_Fix_Indicator[2];
char GGA_Satellites_Used[3];
char GGA_HDOP[4];
char GGA_MSL_Altitude[5];
char GGA_MSL_Units[2];
char GGA_Geoid_Separation[6];
char GGA_Geoid_Units[2];



uint8_t GPS_ReadSentance(char Sentance[]){
  static uint8_t i = 0;
  while(GPS_Serial.available()){
    char buffer = GPS_Serial.read();
    Sentance[i] = buffer;
    i++;
    if(buffer == '\n'){
      Sentance[i] = '\0';
      i = 0;
      return 1;
    }
  }
  return 0;

}

uint8_t GPS_ParseSentance(char Sentance[80]){
  char* GGA_Sentance[]{
    GGA_Time,
    GGA_Latitude,
    GGA_NS_Indicator,
    GGA_Longitude,
    GGA_EW_Indicator,
    GGA_Position_Fix_Indicator,
    GGA_Satellites_Used,
    GGA_HDOP,
    GGA_MSL_Altitude,
    GGA_MSL_Units,
    GGA_Geoid_Separation,
    GGA_Geoid_Units,
    NULL,
    NULL,
  };


  uint8_t index;
  char TestCase[] = "$GPGGA,";
  //Check if GGA Sentance first;
  for (index= 0; TestCase[index] != ','; index++){
    if (TestCase[index] != Sentance[index]){
      return 255;
    }
  }
  index++;
  
  //Start putting things into arrays
  uint8_t GGA_Array_Index = 0, i = 0;
  while(Sentance[index] != '*'){
    char ReadChar = Sentance[index];
    if(GGA_Sentance[GGA_Array_Index] == NULL){
      
      return 1;
    }

    if(ReadChar == ','){
      GGA_Sentance[GGA_Array_Index][i] = '\0';
      GGA_Array_Index++;
      i = 0;
    }else{
      GGA_Sentance[GGA_Array_Index][i] = ReadChar;
      i++;
    }
    index++;
  }

  return 0;

}

uint8_t GPS_CaluclateData(GPS_Position* Position){

  if(GGA_Position_Fix_Indicator[0] == '0'){
    Position->fix = 0;
    return 1;
  }
  Position->fix = 1;



  {
    double Latitude;
    char decimal[3] = {GGA_Latitude[0], GGA_Latitude[1], '\0'};
    GGA_Latitude[0] = ' ';
    GGA_Latitude[1] = ' ';

    Latitude = atoi(decimal) + (atof(GGA_Latitude) / 60.0);
    if (GGA_NS_Indicator[0] == 'S'){
      Latitude = Latitude * -1;
    }

    Position->latitude = Latitude;
  }

  {
    double Longitude;
    char decimal[4] = {GGA_Longitude[0], GGA_Longitude[1], GGA_Longitude[2], '\0'};
    GGA_Longitude[0] = ' ';
    GGA_Longitude[1] = ' ';
    GGA_Longitude[2] = ' ';

    Longitude = atoi(decimal) + (atof(GGA_Longitude) / 60.0);
  
    if (GGA_EW_Indicator[0] == 'W'){
      Longitude = Longitude * -1;
    }

    Position->longitude = Longitude;
  }

  return 0;
}


void GPS_StartUp(uint8_t RX_GPS, uint8_t TX_GPS){
  //For Communcation with GPS
  delay(1000);
  GPS_Serial.begin(9600, SERIAL_8N1, RX_GPS, TX_GPS);

  //To tell the GPS to send updates every second and only 1 type of sentance
  GPS_Serial.write("$PMTK220,1000*2F\r\n");
  delay(1000);
  
  GPS_Serial.write("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
  delay(1000);
}
