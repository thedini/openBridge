#include "openbridge.h"

String read_in;
String parsed[60];

void setup() {
      init_nmea2000();
      Serial.begin(115200);
}
void loop() {
      // if there's any serial available, read it:break;
      while (Serial.available() > 0) {
            read_in = Serial.readStringUntil('=');
            parseVals(read_in, parsed);
            send_msg(parsed);
      }
      parse_nmea();
      delay(1000);
}
