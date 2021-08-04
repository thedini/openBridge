#include <N2kMessages.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <NMEA2000_Teensyx.h>

// UID values
#define TEMP 0
#define BAT  1
#define WIND 2
#define HUMID 3
#define DEPTH 4

// Period definitions
#define tempPeriod 2000
#define batteryPeriod 1000
#define windPeriod 1000
#define humidityPeriod 1000
#define depthPeriod 1000

// These need to be defined in advance in order to store the values and allow for the data to be refreshed even if new data is not recieved.
double cabin_temp = 0;
double voltage = 0;
double current = 0;
double bat_temp = 0;
double wind_speed = 0;
double wind_dir = 0;
double humidity_val = 0;
double depth_val = 0;



// Find the following PGNs from https://www.nmea.org/content/STANDARDS/nmea_2000_pgn__search
const unsigned long tempPGN[] PROGMEM={130310L,130311L,130312L,0};
const unsigned long batPGN[] PROGMEM={127506L,127508L,127513L,0};
const unsigned long windPGN[] PROGMEM={130306L,0};
const unsigned long humidPGN[] PROGMEM={130311L, 0};
const unsigned long depthPGN[] PROGMEM={128267L, 0}; // 



void setup() {
  Serial.begin(9600);
  NMEA2000.SetDeviceCount(5);

  // Set up interface for Temp sensor
  NMEA2000.SetProductInformation("112233",
                                 135,
                                 "TEMP",
                                 "1.0",
                                 "1.0",
                                 0xff,
                                 0xffff,
                                 0xff,
                                 TEMP
                                 );
  NMEA2000.SetDeviceInformation(112233,
                                130,
                                75,
                                135,                             
                                4,
                                TEMP
                               );

  // Set up the interface for the Bat sensor
  NMEA2000.SetProductInformation("112234",
                                 176,
                                 "BAT",
                                 "1.0",
                                 "1.0",
                                 0xff,
                                 0xffff,
                                 0xff,
                                 BAT
                                 );
  NMEA2000.SetDeviceInformation(112234,
                                130,
                                35,
                                176,                    
                                4,
                                BAT
                               );

  // Set up interface for the wind sensor
  NMEA2000.SetProductInformation("112235",
                                 135,
                                 "WIND",
                                 "1.0",
                                 "1.0",
                                 0xff,
                                 0xffff,
                                 0xff,
                                 WIND
                                 );
  NMEA2000.SetDeviceInformation(112234,
                                130,
                                85,
                                135,                         
                                4,
                                WIND
                               );

  // Set up information for the humidity sensor
  NMEA2000.SetProductInformation("112236",
                                 135,
                                 "HUMIDITY",
                                 "1.0",
                                 "1.0",
                                 0xff,
                                 0xffff,
                                 0xff,
                                 HUMID
                                 );
  NMEA2000.SetDeviceInformation(112234,
                                170,
                                75,
                                135,                             
                                4,
                                HUMID
                               );

  // set up information for the depth sensor
  NMEA2000.SetProductInformation("112237",
                                 1851,
                                 "DEPTH",
                                 "1.0",
                                 "1.0",
                                 0xff,
                                 0xffff,
                                 0xff,
                                 DEPTH
                                 );
  NMEA2000.SetDeviceInformation(112234,
                                130,
                                60,
                                1851,                             
                                4,
                                DEPTH
                               );

  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode,22);
  NMEA2000.ExtendTransmitMessages(tempPGN,TEMP);
  NMEA2000.ExtendTransmitMessages(batPGN,BAT);
  NMEA2000.ExtendTransmitMessages(windPGN,WIND);
  NMEA2000.ExtendTransmitMessages(humidPGN,HUMID);
  NMEA2000.ExtendTransmitMessages(depthPGN,DEPTH);
  NMEA2000.Open();
}

void loop() {

    updateValues();
    sendTemperature();
    sendBattery();
    sendWind();
    sendHumidity();
    sendDepth();
    NMEA2000.ParseMessages();
}

// Gets updated values from serial input
void updateValues(){
    int uid;
    String passed_data;
    String cleaned;
    while(Serial.available() > 0){
        passed_data = Serial.readString();
        uid = passed_data.substring(0, passed_data.indexOf(",")-1).toInt();
        switch(uid){
            case 0:
                cabin_temp = passed_data.substring(passed_data.indexOf(",")+1).toFloat();
                break;
            case 1:
                cleaned = passed_data.substring(0, passed_data.indexOf(",")+1);
                voltage = cleaned.substring(0, cleaned.indexOf(",")-1).toFloat();
                current = cleaned.substring(cleaned.indexOf(",")+1, cleaned.lastIndexOf(",")-1).toFloat();
                bat_temp = cleaned.substring(cleaned.lastIndexOf(",")+1).toFloat();
                break;
            case 2:
                cleaned = passed_data.substring(0, passed_data.indexOf(",")+1);
                wind_speed = cleaned.substring(0, cleaned.indexOf(",")-1).toFloat();
                wind_dir = cleaned.substring(cleaned.lastIndexOf(",")+1).toFloat();
                break;
            case 3:
                humidity_val = passed_data.substring(passed_data.indexOf(",")+1).toFloat();
                break;
            case 4:
                depth_val = passed_data.substring(passed_data.indexOf(",")+1).toFloat();
                break;
            default:
                break;
        }
    }
}

// Sends temperature value to the bus
void sendTemperature() {
  static unsigned long Updated=millis();
  tN2kMsg N2kMsg;

  if ( Updated+tempPeriod<millis() ) {
    Updated=millis();
    SetN2kTemperature(N2kMsg, 1, 1, N2kts_MainCabinTemperature, CToKelvin(cabin_temp));
    NMEA2000.SendMsg(N2kMsg,TEMP);
  }
}

// Send batery values to the buss
void sendBattery() {
  static unsigned long Updated=millis()+100;
  tN2kMsg N2kMsg;

  if ( Updated+batteryPeriod<millis() ) {
    Updated=millis();
    SetN2kDCBatStatus(N2kMsg,1,voltage,current,bat_temp,1);
    NMEA2000.SendMsg(N2kMsg,BAT);
  }
}

// Sends wind speed and direction values
void sendWind(){
  static unsigned long Updated=millis();
  tN2kMsg N2kMsg;

  if ( Updated+windPeriod<millis() ) {
    Updated=millis();
    SetN2kWindSpeed(N2kMsg, 1, wind_speed, DegToRad(wind_dir),N2kWind_Apprent);
    NMEA2000.SendMsg(N2kMsg,WIND);
  }
}

// Sends humidity values
void sendHumidity(){
  static unsigned long Updated=millis();
  tN2kMsg N2kMsg;

  if ( Updated+humidityPeriod<millis() ) {
    Updated=millis();
    SetN2kHumidity(N2kMsg,0xff,1,N2khs_InsideHumidity, humidity_val);
    NMEA2000.SendMsg(N2kMsg,HUMID);
  }
}

// Sends depth values to the bus
void sendDepth(){
  static unsigned long Updated=millis();
  tN2kMsg N2kMsg;

  if ( Updated+depthPeriod<millis() ) {
    Updated=millis();
    SetN2kWaterDepth(N2kMsg, 0, depth_val, 0, 1000);
    NMEA2000.SendMsg(N2kMsg,DEPTH);
  }
}