#define SYSTEM_TIME 0
#define RUDDER 1
#define HEADING 2
#define RATE_OF_TURN 3
#define ATTITUDE 4
#define MAGNETIC_VARIATION 5
#define ENGINE_PARAMS_RAPID 6
#define ENGINE_PARAMS_DYNAMIC 7
#define TRANSMISSION_PARAMS 8
#define ENGINE_TRIP_PARAMS 9
#define FLUID_LEVEL 10
#define DC_DETAILED_STATUS 11
#define CHARGER_STATUS 12
#define BATTERY_STATUS 13
#define BATTERY_CONFIGURATION 14
#define LEEWAY 15
#define BOAT_SPEED 16
#define WATER_DEPTH 17
#define DISTANCE_LOG 18
#define WINDLASS_CONTROL_STATUS 19 //NOT IMPEMENTED YET
#define WINDLASS_OPERATING_STATUS 20 //NOT IMPEMENTED YET
#define WINDLASS_MONITORING_STATUS 21 //NOT IMPEMENTED YET
#define RAPID_LAT_LONG 22
#define RAPID_COG_SOG 23
#define GNSS_POS_DATA 24
#define TIME_OFFSET 25
#define CLASS_A_AIS_REPORT 26
#define CLASS_B_AIS_REPORT 27
#define CROSS_TRACK_ERROR 28
#define NAV_INFO 29
#define WAYPOINT_LIST 30
#define APPEND_WAYPOINT_LIST 31
#define GNSS_DOP_DATA 32
#define GNSS_SATS_IN_VIEW 33
#define SET_SATS_IN_VIEW 34 //NOT IMPEMENTED YET
#define APPEND_SAT_INFO 35 //NOT IMPEMENTED YET
#define AIS_STATIC_CLASS_A 36
#define AIS_STATIC_CLASS_B_A 37
#define AIS_STATIC_CLASS_B_B 38
#define WAYPOINT_LIST_NEW 39
#define WIND_SPEED 40
#define OUTSIDE_ENVIROMENTAL_PARAMS 41
#define ENVIROMENTAL_PARAMS 42
#define TEMPERATURE 43
#define HUMIDITY 44
#define PRESSURE 45
#define SET_PRESSURE 46
#define TEMPERATURE_316 47
#define SMALL_CRAFT 48

#include <N2kMessages.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <NMEA2000_Teensyx.h>
#include<Arduino.h>
#include<String>

tNMEA2000_Teensyx NMEA2000;


void init_nmea2000(){
        NMEA2000.SetProductInformation("0126996",
                                     137,
                                     "Simple wind monitor", 
                                     "1.1.0.22 (2021-12-31)",  
                                     "1.1.0.0 (2021-12-31)" 
                                     );
      // Set device information
      NMEA2000.SetDeviceInformation(2, 
                                    130, 
                                    85, 
                                    137                               
                                   );
      NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,23);
      NMEA2000.EnableForward(false);
      NMEA2000.Open();
}
void parse_nmea(){
  NMEA2000.ParseMessages();
}
void system_time(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      int system_data = parsedVals[2].toInt();
      double system_time = parsedVals[3].toFloat();
      int time_source = parsedVals[4].toInt();
      SetN2kPGN126992(N2kMsg, SID, system_data, system_time, time_source);
      NMEA2000.SendMsg(N2kMsg); 
}
void rudder(String *parsedVals){
      tN2kMsg N2kMsg;
      double rudder_position = parsedVals[1].toFloat();
      unsigned char instance = parsedVals[2].toInt();
      int direction_order = parsedVals[3].toInt();
      double angle_order = parsedVals[4].toFloat();
      SetN2kPGN127245(N2kMsg, rudder_position, instance, direction_order, angle_order);
      NMEA2000.SendMsg(N2kMsg);
}
void heading(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID= parsedVals[1].toInt();
      double heading = parsedVals[2].toFloat();
      double deviation = parsedVals[3].toFloat();
      double variation = parsedVals[4].toFloat();
      int reference = parsedVals[5].toInt();
      SetN2kPGN127250(N2kMsg, SID, heading, deviation, variation, reference);
      NMEA2000.SendMsg(N2kMsg);
}
void rate_of_turn(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID= parsedVals[1].toInt();
      double rate_of_turn = parsedVals[2].toFloat();
      SetN2kPGN127251(N2kMsg, SID, rate_of_turn);
      NMEA2000.SendMsg(N2kMsg);
}
void attitude(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID= parsedVals[1].toInt();
      double yaw = parsedVals[2].toFloat();
      double pitch = parsedVals[3].toFloat();
      double roll = parsedVals[4].toFloat();
      SetN2kPGN127257(N2kMsg, SID, yaw, pitch, roll);
      NMEA2000.SendMsg(N2kMsg);
}
void magnetic_variation(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID= parsedVals[1].toInt();
      int mag_source = parsedVals[2].toInt();
      int days_since = parsedVals[3].toInt();
      double variation = parsedVals[4].toFloat();
      SetN2kPGN127258(N2kMsg, SID, mag_source, days_since, variation);
      NMEA2000.SendMsg(N2kMsg);
}

void engine_params_rapid(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char engine_instance = parsedVals[1].toInt();
      double engine_speed = parsedVals[2].toFloat();
      double engine_boost = parsedVals[3].toFloat();
      int engine_tilt_trim = parsedVals[4].toInt();
      SetN2kPGN127488(N2kMsg, engine_instance, engine_speed, engine_boost, engine_tilt_trim);
      NMEA2000.SendMsg(N2kMsg);
}

void engine_params_dynamic(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char engine_instance = parsedVals[1].toInt();
      double oil_preas = parsedVals[2].toFloat();
      double oil_temp = parsedVals[3].toFloat();
      double coolant_temp = parsedVals[4].toFloat();
      double alternator_voltage = parsedVals[5].toFloat();
      double fuel_rate = parsedVals[6].toFloat();
      double engine_hours = parsedVals[7].toFloat();
      double coolant_preas = parsedVals[8].toFloat();
      double fuel_preas = parsedVals[9].toFloat();
      int engine_load = parsedVals[10].toInt();
      int engine_torque = parsedVals[11].toInt();
      int status_1 = parsedVals[12].toInt();
      int status_2 = parsedVals[13].toInt(); 
      SetN2kPGN127489(N2kMsg, engine_instance, oil_preas, oil_temp, coolant_temp,
                      alternator_voltage, fuel_rate, engine_hours, coolant_preas, fuel_preas,
                      engine_load, engine_torque, status_1, status_2);
      NMEA2000.SendMsg(N2kMsg);
}

void transmission_parameters(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char engine_instance = parsedVals[1].toInt();
      int transmission_gear = parsedVals[2].toInt();
      double oil_preas = parsedVals[3].toFloat();
      double oil_temp = parsedVals[4].toFloat();
      unsigned char status_1 = parsedVals[5].toInt();
      SetN2kPGN127493(N2kMsg, engine_instance, transmission_gear, oil_preas, oil_temp, status_1);
      NMEA2000.SendMsg(N2kMsg);
}

void engine_trip_params(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char engine_instance = parsedVals[1].toInt();
      double fuel_used = parsedVals[2].toFloat();
      double fuel_rate = parsedVals[3].toFloat();
      double fuel_economy = parsedVals[4].toFloat();
      double instant_fuel_economy = parsedVals[5].toFloat();
      SetN2kPGN127497(N2kMsg, engine_instance, fuel_used, fuel_rate, fuel_economy, instant_fuel_economy);
      NMEA2000.SendMsg(N2kMsg);
}

void fluid_level(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char instance = parsedVals[1].toInt();
      int fuel_type = parsedVals[2].toInt();
      double level = parsedVals[3].toFloat();
      double capacity = parsedVals[4].toFloat();

      SetN2kPGN127505(N2kMsg, instance, fuel_type, level, capacity);
      NMEA2000.SendMsg(N2kMsg);
}

void dc_detail_status(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      unsigned char dc_instance = parsedVals[2].toInt();
      int dc_type = parsedVals[3].toInt();
      unsigned char state_of_charge = parsedVals[4].toInt();
      unsigned char state_of_health = parsedVals[5].toInt();
      double time_remaining = parsedVals[6].toFloat();
      double ripple_voltage = parsedVals[7].toFloat();
      double capacity = parsedVals[8].toFloat();
      SetN2kPGN127506(N2kMsg, SID, dc_instance, dc_type, state_of_charge, state_of_health, time_remaining, ripple_voltage, capacity);
      NMEA2000.SendMsg(N2kMsg); 
}

void charger_status(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char instance = parsedVals[1].toInt();
      unsigned char battery_instance = parsedVals[2].toInt();
      int charge_state = parsedVals[3].toInt();
      int charger_mode = parsedVals[4].toInt();
      int enabled = parsedVals[5].toInt();
      int equilizer_pending = parsedVals[6].toInt();
      double equalization_time_remaining = parsedVals[7].toFloat();
      SetN2kPGN127507(N2kMsg, instance, battery_instance, charge_state, charger_mode, enabled, equilizer_pending, equalization_time_remaining);
      NMEA2000.SendMsg(N2kMsg); 
}

void battery_status(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char instance = parsedVals[1].toInt();
      double voltage = parsedVals[2].toFloat();
      double current = parsedVals[3].toFloat();
      double temp = parsedVals[4].toFloat();
      unsigned char SID = parsedVals[5].toInt();
      SetN2kPGN127508(N2kMsg, instance, voltage, current, temp, SID);
      NMEA2000.SendMsg(N2kMsg); 
}

void battery_configuration(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char instance = parsedVals[1].toInt();
      int type = parsedVals[2].toInt();
      int supports_equal = parsedVals[3].toInt();
      int nominal_voltage = parsedVals[4].toInt();
      int chemistry = parsedVals[5].toInt();
      double capacity = parsedVals[6].toInt();
      int temp_coefficient = parsedVals[7].toInt();
      double peukert_exponent = parsedVals[8].toInt();
      int charge_efficiency = parsedVals[9].toInt();
      SetN2kPGN127513(N2kMsg, instance, type, supports_equal, nominal_voltage, chemistry, capacity, temp_coefficient, peukert_exponent, charge_efficiency);
      NMEA2000.SendMsg(N2kMsg); 
}

void leeway(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      double leeway = parsedVals[2].toFloat();
      SetN2kPGN128000(N2kMsg, SID, leeway);
      NMEA2000.SendMsg(N2kMsg); 
}

void boat_speed(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      double water_referenced = parsedVals[2].toFloat();
      double ground_referenced = parsedVals[3].toFloat();
      int reference_type = parsedVals[4].toInt();
      SetN2kPGN128259(N2kMsg, SID, water_referenced, ground_referenced, reference_type);
      NMEA2000.SendMsg(N2kMsg); 
}


void water_depth(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      double depth = parsedVals[2].toFloat();
      double my_offset = parsedVals[3].toFloat();
      double range = parsedVals[4].toFloat();
      
      SetN2kPGN128267(N2kMsg, SID, depth, my_offset, range);
      NMEA2000.SendMsg(N2kMsg); 
}


void distance_log(String *parsedVals){
      tN2kMsg N2kMsg;
      int days_since_1970 = parsedVals[1].toInt();
      double seconds_since = parsedVals[2].toFloat();
      int log = parsedVals[3].toInt();
      int trip_log = parsedVals[4].toInt();
      
      SetN2kPGN128275(N2kMsg, days_since_1970, seconds_since, log, trip_log);
      NMEA2000.SendMsg(N2kMsg); 
}


// TODO: Program in the Anchor Windlass control Status
// TODO: Program in the Anchor Windlass Operation Status
// TODO: Program in the Anchor Windlass Monitoring Status

void rapid_lat_log(String *parsedVals){
      tN2kMsg N2kMsg;
      double lat = parsedVals[1].toFloat();
      double longitude = parsedVals[2].toFloat();
      
      SetN2kPGN129025(N2kMsg, lat, longitude);
      NMEA2000.SendMsg(N2kMsg); 
}
void rapid_cog_sog(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      int reference = parsedVals[2].toInt();
      double cog = parsedVals[3].toFloat();
      double sog = parsedVals[4].toFloat();
      
      SetN2kPGN129026(N2kMsg, SID, reference, cog, sog);
      NMEA2000.SendMsg(N2kMsg); 
}

void gnss_position_data(String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      int days_since_1970 = parsedVals[2].toInt();
      double seconds_since_midnight = parsedVals[3].toFloat();
      double latitude = parsedVals[4].toFloat();
      double longitude = parsedVals[5].toFloat();
      double altitude = parsedVals[6].toFloat();
      int type = parsedVals[7].toInt();
      int method = parsedVals[8].toInt();
      unsigned char satellites = parsedVals[9].toInt();
      double hdop = parsedVals[10].toFloat();
      double pdop = parsedVals[11].toFloat();
      double geoidal_separation = parsedVals[12].toFloat();
      unsigned char reference_stations = parsedVals[13].toInt();
      int reference_station_type = parsedVals[14].toInt();
      int reference_station_id = parsedVals[15].toInt();
      
      SetN2kPGN129029(N2kMsg, SID, days_since_1970, seconds_since_midnight,
                        latitude, longitude, altitude, type, method,
                        satellites, hdop, pdop, geoidal_separation,
                        reference_stations, reference_station_type,
                        reference_station_id );
      NMEA2000.SendMsg(N2kMsg); 
}
void time_offset(String *parsedVals){
      tN2kMsg N2kMsg;
      int days_since_1970 = parsedVals[1].toInt();
      double seconds_since_midnight = parsedVals[2].toFloat();
      int local_offset = parsedVals[3].toInt();
      
      SetN2kPGN129033(N2kMsg, days_since_1970, seconds_since_midnight,
                        local_offset);
      NMEA2000.SendMsg(N2kMsg); 
}

void class_a_ais_report(String *parsedVals){
      tN2kMsg N2kMsg;
      int message_id = parsedVals[1].toInt();
      int repeat = parsedVals[2].toInt();
      int user_id = parsedVals[3].toInt();
      double latitude = parsedVals[4].toFloat();
      double longitude = parsedVals[5].toFloat();
      // GOnna make the assumption here that we can pass in an integer
      bool accuracy = parsedVals[6].toInt();
      bool raim = parsedVals[7].toInt();
      int seconds = parsedVals[8].toInt();
      double cog = parsedVals[9].toFloat();
      double sog = parsedVals[10].toFloat();
      double heading = parsedVals[11].toFloat();
      double rot = parsedVals[12].toFloat();
      int nav_status = parsedVals[13].toFloat();
      SetN2kPGN129038(N2kMsg, message_id, repeat, user_id, latitude,
                        longitude, accuracy, raim, seconds, cog,
                        sog, heading, rot, nav_status);
      NMEA2000.SendMsg(N2kMsg); 
}


void class_b_ais_report(String *parsedVals){
      tN2kMsg N2kMsg;
      int message_id = parsedVals[1].toInt();
      int repeat = parsedVals[2].toInt();
      int user_id = parsedVals[3].toInt();
      double latitude = parsedVals[4].toFloat();
      double longitude = parsedVals[5].toFloat();
      // GOnna make the assumption here that we can pass in an integer
      bool accuracy = parsedVals[6].toInt();
      bool raim = parsedVals[7].toInt();
      int seconds = parsedVals[8].toInt();
      double cog = parsedVals[9].toFloat();
      double sog = parsedVals[10].toFloat();
      double heading = parsedVals[11].toFloat();
      int unit = parsedVals[12].toInt();
      bool display = parsedVals[13].toInt();
      bool dsc  = parsedVals[14].toInt();
      bool band = parsedVals[15].toInt();
      bool msg_22 = parsedVals[16].toInt();
      int mode  = parsedVals[17].toInt();
      bool state = parsedVals[18].toInt();
      SetN2kPGN129039(N2kMsg, message_id, repeat, user_id, latitude,
                        longitude, accuracy, raim, seconds, cog,
                        sog, heading, unit, display, dsc, band, msg_22,
                        mode, state);
      NMEA2000.SendMsg(N2kMsg); 
}
void cross_track_error( String *parsedVals){
      tN2kMsg N2kMsg;
      int SID = parsedVals[1].toInt();
      int xte_mode = parsedVals[2].toInt();
      bool nav_terminated = parsedVals[3].toInt();
      double xte = parsedVals[4].toFloat();
      SetN2kPGN129283(N2kMsg, SID, xte_mode, nav_terminated, xte);
      NMEA2000.SendMsg(N2kMsg);
}
void nav_info( String *parsedVals){
      tN2kMsg N2kMsg;
      int SID = parsedVals[1].toInt();
      double distance_to_waypoint = parsedVals[2].toFloat();
      int heading_reference = parsedVals[3].toInt();
      bool perpendicular_crossed = parsedVals[4].toInt();
      bool arrival_circule_entered = parsedVals[5].toInt();
      int calc_type = parsedVals[6].toInt();
      double eta = parsedVals[7].toFloat();
      int eta_date = parsedVals[8].toInt();
      double bearing_orgin_to_waypoint = parsedVals[9].toFloat();
      double bearing_position_to_waypoint = parsedVals[10].toFloat();
      int orgin_waypoint_num = parsedVals[11].toInt();
      int destination_waypoint_num = parsedVals[12].toInt();
      double dest_lat = parsedVals[13].toFloat();
      double dest_long = parsedVals[14].toFloat();
      double waypoint_closing_velocity = parsedVals[15].toFloat();
      SetN2kPGN129284(N2kMsg, SID, distance_to_waypoint, heading_reference,
                        perpendicular_crossed, arrival_circule_entered,
                        calc_type, eta, eta_date, bearing_orgin_to_waypoint,
                        bearing_position_to_waypoint, orgin_waypoint_num,
                        destination_waypoint_num, dest_lat, dest_long,
                        waypoint_closing_velocity);
      NMEA2000.SendMsg(N2kMsg);
}

void waypoint_list( String *parsedVals){
      tN2kMsg N2kMsg;
      char *name;
      int start = parsedVals[1].toInt();
      int database = parsedVals[2].toInt();
      int route = parsedVals[3].toInt();
      bool nav_direction = parsedVals[4].toInt();
      bool sup_data = parsedVals[5].toInt();
      parsedVals[6].toCharArray(name, parsedVals[6].length());
      SetN2kPGN129285(N2kMsg, start, database, route, nav_direction,
                        sup_data, name);
      NMEA2000.SendMsg(N2kMsg);
}
void append_waypoint_list( String *parsedVals){
      tN2kMsg N2kMsg;
      char *name;
      int pid = parsedVals[1].toInt();
      parsedVals[2].toCharArray(name, parsedVals[6].length());
      double latitude = parsedVals[3].toFloat();
      double longitude = parsedVals[4].toFloat();
      AppendN2kPGN129285(N2kMsg, pid, name, latitude, longitude);
      NMEA2000.SendMsg(N2kMsg);
}

void gnss_dop_data( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      int desired_mode = parsedVals[2].toInt();
      int actual_mode = parsedVals[3].toInt();
      double hdop = parsedVals[4].toFloat();
      double vdop = parsedVals[5].toFloat();
      double tdop = parsedVals[6].toFloat();
      SetN2kPGN129539(N2kMsg, SID, desired_mode, actual_mode,
                        hdop, vdop, tdop);
      NMEA2000.SendMsg(N2kMsg);
}

void gnss_sats_in_view( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      int mode = parsedVals[2].toInt();
      SetN2kPGN129540(N2kMsg, SID, mode);
      NMEA2000.SendMsg(N2kMsg);
}

void ais_static_class_a( String *parsedVals){
      tN2kMsg N2kMsg;
      char *callsign;
      char *name;
      char *destination;
      int message_id = parsedVals[1].toInt();
      int repeat = parsedVals[2].toInt();
      int user_id = parsedVals[3].toInt();
      int imo_num = parsedVals[4].toInt();
      parsedVals[5].toCharArray(callsign, parsedVals[5].length());
      parsedVals[6].toCharArray(name, parsedVals[6].length());
      int vessel_type = parsedVals[7].toInt();
      double length = parsedVals[8].toFloat();
      double beam = parsedVals[9].toFloat();
      double pos_ref_stbd = parsedVals[10].toFloat();
      double pos_ref_bow = parsedVals[11].toFloat();
      int eta_data = parsedVals[12].toInt();
      double eta  = parsedVals[13].toFloat();
      double draught = parsedVals[14].toFloat();
      parsedVals[15].toCharArray(destination, parsedVals[15].length());
      int ais_version = parsedVals[16].toInt();
      int gnss_type = parsedVals[17].toInt();
      int dte = parsedVals[18].toInt();
      int ais_info = parsedVals[19].toInt();
      SetN2kPGN129794(N2kMsg, message_id, repeat, user_id, imo_num, callsign, name, vessel_type,
                        length, beam, pos_ref_stbd, pos_ref_bow, eta_data, eta,
                        draught, destination, ais_version, gnss_type, dte, ais_info);
      NMEA2000.SendMsg(N2kMsg);
}
void ais_static_class_b_a( String *parsedVals){
      tN2kMsg N2kMsg;
      char *name;
      int message_id = parsedVals[1].toInt();
      int repeat = parsedVals[2].toInt();
      int user_id = parsedVals[3].toInt();
      parsedVals[4].toCharArray(name, parsedVals[4].length());
      SetN2kPGN129809(N2kMsg, message_id, repeat, user_id, name);
      NMEA2000.SendMsg(N2kMsg);
}

void ais_static_class_b_b( String *parsedVals){
      tN2kMsg N2kMsg;
      char *vendor;
      char *callsign;
      int message_id = parsedVals[1].toInt();
      int repeat = parsedVals[2].toInt();
      int user_id = parsedVals[3].toInt();
      int vessel_type = parsedVals[4].toInt();
      parsedVals[5].toCharArray(vendor, parsedVals[5].length());
      parsedVals[6].toCharArray(callsign, parsedVals[6].length());
      
      double length = parsedVals[7].toFloat();
      double beam = parsedVals[8].toFloat();
      double pos_ref_stbd = parsedVals[9].toFloat();
      double pos_ref_bow = parsedVals[10].toFloat();
      int mothership_id = parsedVals[11].toInt();
      SetN2kPGN129810(N2kMsg, message_id, repeat, user_id, vessel_type,vendor,callsign,
                        length, beam, pos_ref_stbd,
                        pos_ref_bow, mothership_id);
      NMEA2000.SendMsg(N2kMsg);
}

void new_waypoint_list( String *parsedVals){
      tN2kMsg N2kMsg;
      int start = parsedVals[1].toInt();
      int num_waypoints = parsedVals[2].toInt();
      int database = parsedVals[3].toInt();
      SetN2kPGN130074(N2kMsg, start, num_waypoints, database);
      NMEA2000.SendMsg(N2kMsg);
}

void wind_speed( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      double wind_speed = parsedVals[2].toFloat();
      double wind_angle = parsedVals[3].toFloat();
      int reference = parsedVals[4].toInt();
      SetN2kWindSpeed(N2kMsg, SID, KnotsToms(wind_speed),  DegToRad(wind_angle),reference);
      NMEA2000.SendMsg(N2kMsg);
}


void outside_enviromental_params( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      double water_temp = parsedVals[2].toFloat();
      double air_temp = parsedVals[3].toFloat();
      double pressure = parsedVals[4].toFloat();
      SetN2kPGN130310(N2kMsg, SID, CToKelvin(water_temp), CToKelvin(air_temp), mBarToPascal(pressure));
      NMEA2000.SendMsg(N2kMsg);
}

void enviromental_params( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      int temp_source = parsedVals[2].toInt();
      double temp = parsedVals[3].toFloat();
      int humidity_source = parsedVals[4].toInt();
      double humidity = parsedVals[5].toFloat();
      double pressure = parsedVals[6].toFloat();
      SetN2kPGN130311(N2kMsg, SID, temp_source, CToKelvin(temp), humidity_source, humidity, mBarToPascal(pressure));
      NMEA2000.SendMsg(N2kMsg);
}

void temperature_312( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      unsigned char instance = parsedVals[2].toInt();
      int source = parsedVals[3].toInt();
      double actual_temp = parsedVals[4].toFloat();
      int set_temp = parsedVals[5].toInt();
      SetN2kPGN130312(N2kMsg, SID, instance, source, CToKelvin(actual_temp), set_temp);
      NMEA2000.SendMsg(N2kMsg);
}

void humidity( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      unsigned char instance = parsedVals[2].toInt();
      int source = parsedVals[3].toInt();
      double actual_humid = parsedVals[4].toFloat();
      int set_humid = parsedVals[5].toInt();
      SetN2kPGN130313(N2kMsg, SID, instance, source, actual_humid, set_humid);
      NMEA2000.SendMsg(N2kMsg);
}
void pressure( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      unsigned char instance = parsedVals[2].toInt();
      int source = parsedVals[3].toInt();
      double pressure = parsedVals[4].toFloat();
      SetN2kPGN130314(N2kMsg, SID, instance, source, mBarToPascal(pressure));
      NMEA2000.SendMsg(N2kMsg);
}

void set_pressure( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      unsigned char instance = parsedVals[2].toInt();
      int source = parsedVals[3].toInt();
      double set_pressure = parsedVals[4].toFloat();
      SetN2kPGN130315(N2kMsg, SID, instance, source, mBarToPascal(set_pressure));
      NMEA2000.SendMsg(N2kMsg);
}
void temperature_316( String *parsedVals){
      tN2kMsg N2kMsg;
      unsigned char SID = parsedVals[1].toInt();
      unsigned char instance = parsedVals[2].toInt();
      int source = parsedVals[3].toInt();
      double actual_temp = parsedVals[4].toFloat();
      int set_temp = parsedVals[5].toInt();
      SetN2kPGN130316(N2kMsg, SID, instance, source, CToKelvin(actual_temp), set_temp);
      NMEA2000.SendMsg(N2kMsg);
}


void small_craft_status( String *parsedVals){
      tN2kMsg N2kMsg;
      int port_trim_tab = parsedVals[1].toInt();
      int stbd_trim_tab = parsedVals[2].toInt();
      SetN2kPGN130576(N2kMsg, port_trim_tab, stbd_trim_tab);
      NMEA2000.SendMsg(N2kMsg);
}




void send_msg(String* parsed){
      switch(parsed[0].toInt()){
            case SYSTEM_TIME:
                  system_time(parsed);
                  break;
            case RUDDER:
                  rudder(parsed);
                  break;
            case HEADING:
                  heading(parsed);
                  break;
            case RATE_OF_TURN:
                  rate_of_turn(parsed);
                  break;
            case ATTITUDE:
                  attitude(parsed);
                  break;
            case MAGNETIC_VARIATION:
                  magnetic_variation(parsed);
                  break;
            case ENGINE_PARAMS_RAPID:
                  engine_params_rapid(parsed);
                  break;
            case ENGINE_PARAMS_DYNAMIC:
                  engine_params_dynamic(parsed);
                  break;
            case TRANSMISSION_PARAMS:
                  transmission_parameters(parsed);
                  break;
            case ENGINE_TRIP_PARAMS:
                  engine_trip_params(parsed);
                  break;
            case FLUID_LEVEL:
                  fluid_level(parsed);
                  break;
            case DC_DETAILED_STATUS:
                  dc_detail_status(parsed);
                  break;
            case CHARGER_STATUS:
                  charger_status(parsed);
                  break;
            case BATTERY_STATUS:
                  battery_status(parsed);
                  break;
            case BATTERY_CONFIGURATION:
                  battery_configuration(parsed);
                  break;
            case LEEWAY:
                  leeway(parsed);
                  break;
            case BOAT_SPEED:
                  boat_speed(parsed);
                  break;
            case WATER_DEPTH:
                  water_depth(parsed);
                  break;
            case DISTANCE_LOG:
                  distance_log(parsed);
                  break;
            case WINDLASS_CONTROL_STATUS:
                  break;
            case WINDLASS_OPERATING_STATUS:
                  break;
            case WINDLASS_MONITORING_STATUS:
                  break;
            case RAPID_LAT_LONG:
                  rapid_lat_log(parsed);
                  break;
            case RAPID_COG_SOG:
                  rapid_cog_sog(parsed);
                  break;
            case GNSS_POS_DATA:
                  gnss_position_data(parsed);
                  break;
            case TIME_OFFSET:
                  time_offset(parsed);
                  break;
            case CLASS_A_AIS_REPORT:
                  class_a_ais_report(parsed);
                  break;
            case CLASS_B_AIS_REPORT:
                  class_b_ais_report(parsed);
                  break;
            case CROSS_TRACK_ERROR:
                  cross_track_error(parsed);
                  break;
            case NAV_INFO:
                  nav_info(parsed);
                  break;
            case WAYPOINT_LIST:
                  waypoint_list(parsed);
                  break;
            case APPEND_WAYPOINT_LIST:
                  append_waypoint_list(parsed);
                  break;
            case GNSS_DOP_DATA:
                  gnss_dop_data(parsed);
                  break;
            case GNSS_SATS_IN_VIEW:
                  gnss_sats_in_view(parsed);
                  break;
            case SET_SATS_IN_VIEW:
                  break;
            case APPEND_SAT_INFO:
                  break;
            case AIS_STATIC_CLASS_A:
                  ais_static_class_a(parsed);
                  break;
            case AIS_STATIC_CLASS_B_A:
                  ais_static_class_b_a(parsed);
                  break;
            case AIS_STATIC_CLASS_B_B:
                  ais_static_class_b_b(parsed);
                  break;
            case WAYPOINT_LIST_NEW:
                  new_waypoint_list(parsed);
                  break;
            case WIND_SPEED:
                  wind_speed(parsed);
                  break;
            case OUTSIDE_ENVIROMENTAL_PARAMS:
                  outside_enviromental_params(parsed);
                  break;
            case ENVIROMENTAL_PARAMS:
                  enviromental_params(parsed);
                  break;
            case TEMPERATURE:
                  temperature_312(parsed);
                  break;
            case HUMIDITY:
                  humidity(parsed);
                  break;
            case PRESSURE:
                  pressure(parsed);
                  break;
            case SET_PRESSURE:
                  set_pressure(parsed);
                  break;
            case TEMPERATURE_316:
                  temperature_316(parsed);
                  break;
            case SMALL_CRAFT:
                  small_craft_status(parsed);
                  break;
            default:
                  break;
      }
}




void parseVals(String stream, String parsed[]) {
  Serial.println(stream);
  int current_cleaned_idx = 0;
    String chopped = stream;
    while(chopped.indexOf(",") != chopped.lastIndexOf(",")){
      if(current_cleaned_idx != 0){
        chopped = chopped.substring(chopped.indexOf(",") + 1, chopped.length());
      }
      String element = chopped.substring(0, chopped.indexOf(","));
      
      parsed[current_cleaned_idx] = element;
      current_cleaned_idx++;
    }
}
