/*
Demo code that sends wind data to the bus. Only works with Maretron at the moment
*/

#include <NMEA2000_Teensyx.h>
#include <N2kMessages.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
//#include <NMEA2000_teensy.h>
//tNMEA2000_teensy NMEA2000 // https://github.com/ttlappalainen/NMEA2000_Teensyx
tNMEA2000_Teensyx NMEA2000;
int speed = 0;
int my_angle = 0;
// List here messages your device will transmit.
const unsigned long TransmitMessages[] PROGMEM={130306L,0};

void setup() {
  // Set Product information
  NMEA2000.SetProductInformation("0126996", // Manufacturer's Model serial code
                                 137, // Manufacturer's product code
                                 "Simple wind monitor",  // Manufacturer's Model ID
                                 "1.1.0.22 (2016-12-31)",  // Manufacturer's Software version code
                                 "1.1.0.0 (2016-12-31)" // Manufacturer's Model version
                                 );
  // Set device information
  NMEA2000.SetDeviceInformation(2,
                                130,
                                85,
                                137                             
                               );
  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,23);
  NMEA2000.EnableForward(false);
  NMEA2000.ExtendTransmitMessages(TransmitMessages);
  NMEA2000.Open();
}


void loop() {
  SendN2kWind();
  NMEA2000.ParseMessages();
}

double ReadWindAngle(int angle) {
  return DegToRad(angle);
}

double ReadWindSpeed(int wind) {
  return (double)wind;
}

#define WindUpdatePeriod 1000

void SendN2kWind() {
  static unsigned long WindUpdated=millis();
  tN2kMsg N2kMsg;
  if ( WindUpdated+WindUpdatePeriod<millis() ) {
    // THese next two variables are just iterators that show the wind changing dynamically
    speed = (speed+1)%50;
    my_angle = (my_angle+5)%360;
    SetN2kWindSpeed(N2kMsg, 1, ReadWindSpeed(speed), ReadWindAngle(my_angle),N2kWind_Apprent);
    WindUpdated=millis();
    // Sends message to the bus
    NMEA2000.SendMsg(N2kMsg);
  }
}
