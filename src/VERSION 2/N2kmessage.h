/*
N2kMessages.h

Copyright (c) 2015-2021 Timo Lappalainen, Kave Oy, www.kave.fi

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


This is collection of functions for handling NMEA2000 bus messages.
Library contains functions to e.g. create message named with PGN like SetN2kPGN129025 and
inline alias for them like SetN2kLatLonRapid.

Each Setxxx funtion sets related message PGN and its default priority. So if you want to change
priority on function, you have to do it after Setxxx call.

If you do not send anything to NMEA2000 bus, you do not need this library. Funtions for
BUS handling PGN:s like 60928 "ISO Address Claim" has been defined in bus device library
NMEA2000.h
*/

#ifndef OPEN_BRIDGE_N2KMESSAGE_H
#define OPEN_BRIDGE_N2KMESSAGE_H



#include "N2kTypes.h"
#include <stdint.h>
#include <vector>
#include <string>
const double   N2kDoubleNA=-1e9;
const float    N2kFloatNA=-1e9;
const uint8_t  N2kUInt8NA=0xff;
const int8_t   N2kInt8NA=0x7f;
const uint16_t N2kUInt16NA=0xffff;
const int16_t  N2kInt16NA=0x7fff;
const uint32_t N2kUInt32NA=0xffffffff;
const int32_t  N2kInt32NA=0x7fffffff;
const uint64_t N2kUInt64NA=0xffffffffffffffffLL;
const int64_t  N2kInt64NA=0x7fffffffffffffffLL;


//*****************************************************************************
// System date/time
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and heading at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - SystemDate            Days since 1970-01-01
//  - SystemTime            seconds since midnight
//  - TimeSource            see tN2kTimeSource
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kSystemTime(std::vector<std::string> &message,unsigned char SID, uint16_t SystemDate,
                             double SystemTime, tN2kTimeSource TimeSource=N2ktimes_GPS);

//*****************************************************************************
// Rudder
// Input:
// - RudderPosition         Current rudder postion in radians.
// - Instance               Rudder instance.
// - RudderDirectionOrder   See tN2kRudderDirectionOrder. Direction, where rudder should be turned.
// - AngleOrder             In radians angle where rudder should be turned.
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kRudder(std::vector<std::string> &message,double RudderPosition, unsigned char Instance=0,
                         tN2kRudderDirectionOrder RudderDirectionOrder=N2kRDO_NoDirectionOrder, double AngleOrder=N2kDoubleNA);


//*****************************************************************************
// Vessel Heading
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and heading at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - Heading               Heading in radians
//  - Deviation             Magnetic deviation in radians. Use N2kDoubleNA for undefined value.
//  - Variation             Magnetic variation in radians. Use N2kDoubleNA for undefined value.
//  - ref                   Heading reference. See definition of tN2kHeadingReference.
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.

void SetN2kTrueHeading(std::vector<std::string> &message,unsigned char SID, double Heading, double Deviation, double Variation);


//*****************************************************************************
// Rate of Turn
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and heading at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - Rate of turn          Change in heading in radians per second
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kRateOfTurn(std::vector<std::string> &message,unsigned char SID, double RateOfTurn);


//*****************************************************************************
// Attitude
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and heading at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - Yaw                   Heading in radians.
//  - Pitch                 Pitch in radians. Positive, when your bow rises.
//  - Roll                  Roll in radians. Positive, when tilted right.
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kAttitude(std::vector<std::string> &message,unsigned char SID, double Yaw, double Pitch, double Roll);

//*****************************************************************************
// Magnetic Variation
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and heading at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - Source                How was the variation value generated
//  - DaysSince1970         Days since January 1, 1970
//  - Variation             Magnetic variation/declination in radians
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kMagneticVariation(std::vector<std::string> &message,unsigned char SID, tN2kMagneticVariation Source, uint16_t DaysSince1970, double Variation);

//*****************************************************************************
// Engine parameters rapid
// Input:
//  - EngineInstance        Engine instance.
//  - EngineSpeed           RPM (Revolutions Per Minute)
//  - EngineBoostPressure   in Pascal
//  - EngineTiltTrim        in %
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kEngineParamRapid(std::vector<std::string> &message,unsigned char EngineInstance, double EngineSpeed,
                                   double EngineBoostPressure=N2kDoubleNA, int8_t EngineTiltTrim=N2kInt8NA);

//*****************************************************************************
// Engine parameters dynamic
// Input:
//  - EngineInstance        Engine instance.
//  - EngineOilPress        in Pascal
//  - EngineOilTemp         in Kelvin
//  - EngineCoolantTemp     in Kelvin
//  - AltenatorVoltage      in Voltage
//  - FuelRate              in litres/hour
//  - EngineHours           in seconds
//  - EngineCoolantPress    in Pascal
//  - EngineFuelPress       in Pascal
//  - EngineLoad            in %
//  - EngineTorque          in %
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kEngineDynamicParam(std::vector<std::string> &message,unsigned char EngineInstance, double EngineOilPress, double EngineOilTemp, double EngineCoolantTemp, double AltenatorVoltage,
                                     double FuelRate, double EngineHours, double EngineCoolantPress=N2kDoubleNA, double EngineFuelPress=N2kDoubleNA,
                                     int8_t EngineLoad=N2kInt8NA, int8_t EngineTorque=N2kInt8NA,
                                     tN2kEngineDiscreteStatus1 Status1=0, tN2kEngineDiscreteStatus2 Status2=0);
//*****************************************************************************
// Transmission parameters, dynamic
// Input:
//  - EngineInstance        Engine instance.
//  - TransmissionGear      Selected transmission. See tN2kTransmissionGear
//  - OilPressure           in Pascal
//  - OilTemperature        in K
//  - EngineTiltTrim        in %
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kTransmissionParameters(std::vector<std::string> &message,unsigned char EngineInstance, tN2kTransmissionGear TransmissionGear,
                                         double OilPressure, double OilTemperature, unsigned char DiscreteStatus1=0);

//*****************************************************************************
// Trip Parameters, Engine
// Input:
//  - EngineInstance           Engine instance.
//  - TripFuelUsed             in litres
//  - FuelRateAverage          in litres/hour
//  - FuelRateEconomy          in litres/hour
//  - InstantaneousFuelEconomy in litres/hour
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kEngineTripParameters(std::vector<std::string> &message,unsigned char EngineInstance, double TripFuelUsed,
                                       double FuelRateAverage,
                                       double FuelRateEconomy=N2kDoubleNA, double InstantaneousFuelEconomy=N2kDoubleNA);
//*****************************************************************************
// Fluid level
// Input:
//  - Instance              Tank instance. Different devices handles this a bit differently. So it is best to have instance unique over
//                          all devices on the bus.
//  - FluidType             Defines type of fluid. See definition of tN2kFluidType
//  - Level                 Tank level in % of full tank.
//  - Capacity              Tank Capacity in litres
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kFluidLevel(std::vector<std::string> &message,unsigned char Instance, tN2kFluidType FluidType, double Level, double Capacity);

//*****************************************************************************
// DC Detailed Status
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and heading at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - DCInstance            DC instance.
//  - DCType                Defines type of DC source. See definition of tN2kDCType
//  - StateOfCharge         % of charge
//  - StateOfHealth         % of heath
//  - TimeRemaining         Time remaining in seconds
//  - RippleVoltage         DC output voltage ripple in V
//  - Capacity              Battery capacity in coulombs
void SetN2kDCStatus(std::vector<std::string> &message,unsigned char SID, unsigned char DCInstance, tN2kDCType DCType,
                           unsigned char StateOfCharge, unsigned char StateOfHealth, double TimeRemaining, double RippleVoltage=N2kDoubleNA, double Capacity=N2kDoubleNA);

//*****************************************************************************
// Charger Status
// Input:
//  - Instance                     ChargerInstance.
//  - BatteryInstance              BatteryInstance.
//  - Operating State              see. tN2kChargeState
//  - Charger Mode                 see. tN2kChargerMode
//  - Charger Enable/Disable       boolean
//  - Equalization Pending         boolean
//  - Equalization Time Remaining  double seconds
//
void SetN2kChargerStatus(std::vector<std::string> &message,unsigned char Instance, unsigned char BatteryInstance,
                                tN2kChargeState ChargeState, tN2kChargerMode ChargerMode=N2kCM_Standalone,
                                tN2kOnOff Enabled=N2kOnOff_On, tN2kOnOff EqualizationPending=N2kOnOff_Unavailable, double EqualizationTimeRemaining=N2kDoubleNA);

//*****************************************************************************
// Battery Status
// Input:
//  - BatteryInstance       BatteryInstance.
//  - BatteryVoltage        Battery voltage in V
//  - BatteryCurrent        Current in A
//  - BatteryTemperature    Battery temperature in K. Use function CToKelvin, if you want to use °C.
//  - SID                   Sequence ID.
void SetN2kDCBatStatus(std::vector<std::string> &message,unsigned char BatteryInstance, double BatteryVoltage, double BatteryCurrent=N2kDoubleNA,
                              double BatteryTemperature=N2kDoubleNA, unsigned char SID=1);


//*****************************************************************************
// Battery Configuration Status
// Note this has not yet confirmed to be right. Specially Peukert Exponent can have in
// this configuration values from 1 to 1.504. And I expect on code that I have to send
// value PeukertExponent-1 to the bus.
// Input:
//  - BatteryInstance       BatteryInstance.
//  - BatType               Type of battery. See definition of tN2kBatType
//  - SupportsEqual         Supports equalization. See definition of tN2kBatEqSupport
//  - BatNominalVoltage     Battery nominal voltage. See definition of tN2kBatNomVolt
//  - BatChemistry          Battery See definition of tN2kBatChem
//  - BatCapacity           Battery capacity in Coulombs. Use AhToCoulombs, if you have your value in Ah.
//  - BatTemperatureCoeff   Battery temperature coefficient in %
//  - PeukertExponent       Peukert Exponent
//  - ChargeEfficiencyFactor Charge efficiency factor
void SetN2kBatConf(std::vector<std::string> &message,unsigned char BatInstance, tN2kBatType BatType, tN2kBatEqSupport SupportsEqual,
                          tN2kBatNomVolt BatNominalVoltage, tN2kBatChem BatChemistry, double BatCapacity, int8_t BatTemperatureCoefficient,
                          double PeukertExponent, int8_t ChargeEfficiencyFactor);

//*****************************************************************************
// Leeway
// Input:
//  - SID            Sequence ID field
//  - Leeway         Nautical Leeway Angle, which is defined as the angle between the vessel’s heading (direction to which the
//                   vessel’s bow points) and its course (direction of its motion (track) through the water)
// Output:
//  - N2kMsg         NMEA2000 message ready to be send.
void SetN2kLeeway(std::vector<std::string> &message,unsigned char SID, double Leeway);

//*****************************************************************************
// Boat speed
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and wind at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - WaterReferenced        Speed over water in m/s
//  - GroundReferenced      Ground referenced speed in m/s
//  - SWRT                  Type of transducer. See definition for tN2kSpeedWaterReferenceType
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kBoatSpeed(std::vector<std::string> &message,unsigned char SID, double WaterReferenced, double GroundReferenced=N2kDoubleNA, tN2kSpeedWaterReferenceType SWRT=N2kSWRT_Paddle_wheel);

//*****************************************************************************
// Water depth
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and depth at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - DepthBelowTransducer  Depth below transducer in meters
//  - Offset                Distance in meters between transducer and surface (std::vector<std::string> &message,positive) or transducer and keel (negative)
//  - Range                 Measuring range
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kWaterDepth(std::vector<std::string> &message,unsigned char SID, double DepthBelowTransducer, double Offset, double Range=N2kDoubleNA);

//*****************************************************************************
// Distance log
// Input:
//  - DaysSince1970         Timestamp
//  - SecondsSinceMidnight  Timestamp
//  - Log                   Total meters travelled
//  - Trip Log              Meters travelled since last reset
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kDistanceLog(std::vector<std::string> &message,uint16_t DaysSince1970, double SecondsSinceMidnight, uint32_t Log, uint32_t TripLog);


//*****************************************************************************
// PGN128776 - Anchor Windlass Control Status
//
// Eg: SetN2kPGN128776(N2kMsg, SID, WindlassIdentifier, WindlassDirectionControl, SpeedControl);
// -- SID - message sequence id - use to tie together windlass status msgs
// -- WindlassIdentifier - instance of the windlass being reported
// -- WindlassDirectionControl - see tN2kWindlassDirectionControl
// -- SpeedControl - Single speed 0=off, 1-100=on; Dual speed: 0=off, 1-49=slow, 50-100=fast; Proporional speed: 0=off, 1-100=speed
// -- SpeedControlType (optional) - see tN2kSpeedType
// -- AnchorDockingControl (optional) - see tN2kGenericStatusPair
// -- PowerEnable (optional) - see tN2kDGenericStatusPair
// -- MechanicalLock (optional) - see tN2kDGenericStatusPair
// -- DeckAndAnchorWash (optional) - see tN2kDGenericStatusPair
// -- AnchorLight (optional) - see tN2kDGenericStatusPair
// -- CommandTimeout (optional) - range 0.25 ... 1.26 seconds
// -- WindlassControlEvents (optional) - see tN2kWindlassControlEvents

void SetN2kPGN128776(std::vector<std::string> &message,
        unsigned char SID,
        unsigned char WindlassIdentifier,
        tN2kWindlassDirectionControl WindlassDirectionControl,
        unsigned char SpeedControl,
        tN2kSpeedType SpeedControlType = N2kDD488_DataNotAvailable,
        tN2kGenericStatusPair AnchorDockingControl = N2kDD002_Unavailable,
        tN2kGenericStatusPair PowerEnable = N2kDD002_Unavailable,
        tN2kGenericStatusPair MechanicalLock = N2kDD002_Unavailable,
        tN2kGenericStatusPair DeckAndAnchorWash = N2kDD002_Unavailable,
        tN2kGenericStatusPair AnchorLight = N2kDD002_Unavailable,
        double CommandTimeout = 0.4,
        const tN2kWindlassControlEvents &WindlassControlEvents = tN2kWindlassControlEvents());


//*****************************************************************************
// PGN128777 - Anchor Windlass Operating Status
//
// Eg: SetN2kPGN128777(N2kMsg, SID, WindlassIdentifier, RodeCounterValue, WindlassLineSpeed);
// -- SID - message sequence id - use to tie together windlass status msgs
// -- WindlassIdentifier - instance of the windlass being reported
// -- RodeCounterValue - amount of rode deployed in metres
// -- WindlassLineSpeed (optional) - deployment speed in metres per second
// -- WindlassMotionStatus (optional) - see tN2kWindlassMotionStates
// -- RodeTypeStatus (optional) - see tN2kRodeTypeStates
// -- AnchorDockingStatus (optional) - see tN2kAnchorDockingStates
// -- WindlassOperatingEvents (optional) -- see tN2kWindlassOperatingEvents

void SetN2kPGN128777(std::vector<std::string> &message,
        unsigned char SID,
        unsigned char WindlassIdentifier,
        double RodeCounterValue,
        double WindlassLineSpeed = N2kDoubleNA,
        tN2kWindlassMotionStates WindlassMotionStatus = N2kDD480_Unavailable,
        tN2kRodeTypeStates RodeTypeStatus = N2kDD481_Unavailable,
        tN2kDD482 AnchorDockingStatus = N2kDD482_DataNotAvailable,
        const tN2kWindlassOperatingEvents &WindlassOperatingEvents = tN2kWindlassOperatingEvents());


//*****************************************************************************
// PGN128778 - Anchor Windlass Monitoring Status
//
// Eg: SetN2kPGN128778(N2kMsg, SID, WindlassIdentifier, TotalMotorTime);
// -- SID - message sequence id - use to tie together windlass status msgs
// -- WindlassIdentifier - instance of the windlass being reported
// -- TotalMotorTime - total run time of the motor in seconds
// -- ControllerVoltage (optional) - voltage in Volts
// -- MotorCurrent (optional) - current in Amperes
// -- WindlassMonitoringEvents (optional) - see tN2kWindlassMonitoringEvents

void SetN2kPGN128778(std::vector<std::string> &message,
        unsigned char SID,
        unsigned char WindlassIdentifier,
        double TotalMotorTime,
        double ControllerVoltage = N2kDoubleNA,
        double MotorCurrent = N2kDoubleNA,
        const tN2kWindlassMonitoringEvents &WindlassMonitoringEvents = tN2kWindlassMonitoringEvents());


//*****************************************************************************
// Lat/lon rapid
// Input:
//  - Latitude               Latitude in degrees
//  - Longitude              Longitude in degrees
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kLatLonRapid(std::vector<std::string> &message,double Latitude, double Longitude);

//*****************************************************************************
// COG SOG rapid
// Input:
//  - COG                   Cource Over Ground in radians
//  - SOG                   Speed Over Ground in m/s
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kCOGSOGRapid(std::vector<std::string> &message,unsigned char SID, tN2kHeadingReference ref, double COG, double SOG);


//*****************************************************************************
// GNSS Position Data
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and GPS at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - DaysSince1970         Days since 1.1.1970. You can find sample how to convert e.g. NMEA0183 date info to this on my NMEA0183 library on
//                          NMEA0183Messages.cpp on function NMEA0183ParseRMC_nc
//  - Latitude              Latitude in degrees
//  - Longitude             Longitude in degrees
//  - Altitude              Altitude in meters
//  - GNSStype              GNSS type. See definition of tN2kGNSStype
//  - GNSSmethod            GNSS method type. See definition of tN2kGNSSmethod
//  - nSatellites           Number of satellites used for data
//  - HDOP                  Horizontal Dilution Of Precision in meters.
//  - PDOP                  Probable dilution of precision in meters.
//  - GeoidalSeparation     Geoidal separation in meters
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kGNSS(std::vector<std::string> &message,unsigned char SID, uint16_t DaysSince1970, double SecondsSinceMidnight,
                       double Latitude, double Longitude, double Altitude,
                       tN2kGNSStype GNSStype, tN2kGNSSmethod GNSSmethod,
                       unsigned char nSatellites, double HDOP, double PDOP=0, double GeoidalSeparation=0,
                       unsigned char nReferenceStations=0, tN2kGNSStype ReferenceStationType=N2kGNSSt_GPS, uint16_t ReferenceStationID=0,
                       double AgeOfCorrection=0);

//*****************************************************************************
// Date,Time & Local offset  ( see also PGN 126992 )
// Input:
//  - DaysSince1970         Days since 1.1.1970. You can find sample how to convert e.g. NMEA0183 date info to this on my NMEA0183 library on
//                          NMEA0183Messages.cpp on function NMEA0183ParseRMC_nc
//  - Time                  Seconds since midnight
//  - Local offset          Local offset in minutes
void SetN2kLocalOffset(std::vector<std::string> &message,uint16_t DaysSince1970, double SecondsSinceMidnight, int16_t LocalOffset);

//*****************************************************************************
// AIS position reports for Class A
// Input:
//  - N2kMsg                NMEA2000 message to decode
void SetN2kAISClassAPosition(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID, double Latitude, double Longitude,
                                    bool Accuracy, bool RAIM, uint8_t Seconds, double COG, double SOG, double Heading, double ROT, tN2kAISNavStatus NavStatus);

//*****************************************************************************
// AIS position reports for Class B
// Input:
//  - N2kMsg                NMEA2000 message to decode
void SetN2kAISClassBPosition(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID,
                                    double Latitude, double Longitude, bool Accuracy, bool RAIM,
                                    uint8_t Seconds, double COG, double SOG, double Heading, tN2kAISUnit Unit,
                                    bool Display, bool DSC, bool Band, bool Msg22, tN2kAISMode Mode, bool State);

//*****************************************************************************
// Cross Track Error
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kXTE(std::vector<std::string> &message,unsigned char SID, tN2kXTEMode XTEMode, bool NavigationTerminated, double XTE);

//*****************************************************************************
// Navigation info
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kNavigationInfo(std::vector<std::string> &message,unsigned char SID, double DistanceToWaypoint, tN2kHeadingReference BearingReference,
                                 bool PerpendicularCrossed, bool ArrivalCircleEntered, tN2kDistanceCalculationType CalculationType,
                                 double ETATime, int16_t ETADate, double BearingOriginToDestinationWaypoint, double BearingPositionToDestinationWaypoint,
                                 uint8_t OriginWaypointNumber, uint8_t DestinationWaypointNumber,
                                 double DestinationLatitude, double DestinationLongitude, double WaypointClosingVelocity);
//*****************************************************************************
// Waypoint list
// Input:
//  - Start                 The ID of the first waypoint
//  - Database              Database ID
//  - Route                 Route ID
//  - RouteName             The name of the current route
//  - ID                    The ID of the current waypoint
//  - Name                  The name of the current waypoint
//  - Latitude              The latitude of the current waypoint
//  - Longitude             The longitude of the current waypoint
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kPGN129285(std::vector<std::string> &message,uint16_t Start, uint16_t Database, uint16_t Route,
                     bool NavDirection, bool SupplementaryData, char* RouteName);

bool AppendN2kPGN129285(std::vector<std::string> &message,uint16_t WPID2, char* WPName2, double Latitude2, double Longitude2);

//*****************************************************************************
// GNSS DOP data
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and GPS at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - DesiredMode           Desired DOP mode.
//  - ActualMode            Actual DOP mode.
//  - HDOP                  Horizontal Dilution Of Precision in meters.
//  - PDOP                  Probable dilution of precision in meters.
//  - TDOP                  Time dilution of precision
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kGNSSDOPData(std::vector<std::string> &message,unsigned char SID, tN2kGNSSDOPmode DesiredMode, tN2kGNSSDOPmode ActualMode,
                              double HDOP, double VDOP, double TDOP);

//*****************************************************************************
struct tSatelliteInfo {
    unsigned char PRN;
    double Elevation;
    double Azimuth;
    double SNR;
    double RangeResiduals;
    tN2kPRNUsageStatus UsageStatus;
};

//*****************************************************************************
// GNSS Satellites in View
// Use first function for basic initialization and second for adding satellites.
//
// Initialize GNSS Satellites in View message
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and GPS at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - Mode                  Range residual mode.
//  - SatelliteInfo         Satellite info see tSatelliteInfo above.
void SetN2kPGN129540(std::vector<std::string> &message,unsigned char SID=0xff, tN2kRangeResidualMode Mode=N2kDD072_Unavailable);

// Append new satellite info for GNSS Satellites in View message
// Input:
//  - SatelliteInfo         Requested satellite info.
//
// Return:
//   true  - if function succeeds.
//   false - if no more satellites can be added or try to use wrong or uninitialized message.
void SetN2kGNSSSatellitesInView(std::vector<std::string> &message,unsigned char SID=0xff, tN2kRangeResidualMode Mode=N2kDD072_Unavailable);
bool AppendSatelliteInfo(std::vector<std::string> &message,const tSatelliteInfo& SatelliteInfo);


//*****************************************************************************
// AIS static data class A
// Input:
//  - MessageID             Message type
//  - Repeat                Repeat indicator
//  - UserID                MMSI
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kAISClassAStatic(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID,
                                  uint32_t IMOnumber, char *Callsign, char *Name, uint8_t VesselType, double Length,
                                  double Beam, double PosRefStbd, double PosRefBow, uint16_t ETAdate, double ETAtime,
                                  double Draught, char *Destination, tN2kAISVersion AISversion, tN2kGNSStype GNSStype,
                                  tN2kAISDTE DTE, tN2kAISTranceiverInfo AISinfo);


//*****************************************************************************
// AIS static data class B part A
// Input:
//  - MessageID             Message type
//  - Repeat                Repeat indicator
//  - UserID                MMSI
//  - Name                  Vessel name
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kAISClassBStaticPartA(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID, char *Name);

//*****************************************************************************
// AIS static data class B part B
// Input:
//  - MessageID             Message type
//  - Repeat                Repeat indicator
//  - UserID                MMSI
//  - Name                  Vessel name
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kAISClassBStaticPartB(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID,
                                       uint8_t VesselType, char *Vendor, char *Callsign, double Length, double Beam,
                                       double PosRefStbd, double PosRefBow, uint32_t MothershipID);

//*****************************************************************************
// Waypoint list
// Input:
//  - Start                 The ID of the first waypoint
//    NumItems
//  - NumWaypoints          Number of valid WPs in the WP-List
//  - Database              Database ID
//  - ID                    The ID of the current waypoint
//  - Name                  The name of the current waypoint
//  - Latitude              The latitude of the current waypoint
//  - Longitude             The longitude of the current waypoint
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kWaypointList(std::vector<std::string> &message,uint16_t Start, uint16_t NumWaypoints, uint16_t Database);

//*****************************************************************************
// Wind Speed
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and wind at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - WindSpeed             Measured wind speed in m/s
//  - WindAngle             Measured wind angle in radians. If you have value in degrees, use function DegToRad(myval) in call.
//  - WindReference         Wind reference, see definition of tN2kWindReference
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kWindSpeed(std::vector<std::string> &message,unsigned char SID, double WindSpeed, double WindAngle, int WindReference);

//*****************************************************************************
// Outside Environmental parameters
// Input:
//  - SID                   Sequence ID.
//  - WaterTemperature      Water temperature in K. Use function CToKelvin, if you want to use °C.
//  - OutsideAmbientAirTemperature      Outside ambient temperature in K. Use function CToKelvin, if you want to use °C.
//  - AtmosphericPressure   Atmospheric pressure in Pascals. Use function mBarToPascal, if you like to use mBar
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kOutsideEnvironmentalParameters(std::vector<std::string> &message,unsigned char SID, double WaterTemperature,
                                                 double OutsideAmbientAirTemperature=N2kDoubleNA, double AtmosphericPressure=N2kDoubleNA);

//*****************************************************************************
// Environmental parameters
// Note that in PGN 130311 TempInstance is as TempSource in PGN 130312. I do not know why this
// renaming is confusing.
// Pressure has to be in pascal. Use function mBarToPascal, if you like to use mBar
// Input:
//  - SID                   Sequence ID.
//  - TempSource            see tN2kTempSource
//  - Temperature           Temperature in K. Use function CToKelvin, if you want to use °C.
//  - HumiditySource        see tN2kHumiditySource.
//  - Humidity              Humidity in %
//  - AtmosphericPressure   Atmospheric pressure in Pascals. Use function mBarToPascal, if you like to use mBar
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kEnvironmentalParameters(std::vector<std::string> &message,unsigned char SID, tN2kTempSource TempSource, double Temperature,
                                          tN2kHumiditySource HumiditySource=N2khs_Undef, double Humidity=N2kDoubleNA, double AtmosphericPressure=N2kDoubleNA);

//*****************************************************************************
// Temperature
// Temperatures should be in Kelvins
// Input:
//  - SID                   Sequence ID.
//  - TempInstance          This should be unic at least on one device. May be best to have it unic over all devices sending this PGN.
//  - TempSource            see tN2kTempSource
//  - ActualTemperature     Temperature in K. Use function CToKelvin, if you want to use °C.
//  - SetTemperature        Set temperature in K. Use function CToKelvin, if you want to use °C. This is meaningfull for temperatures,
//                          which can be controlled like cabin, freezer, refridgeration temperature. God can use value for this for
//                          outside and sea temperature values.
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kTemperature(std::vector<std::string> &message,unsigned char SID, unsigned char TempInstance, tN2kTempSource TempSource,
                              double ActualTemperature, double SetTemperature=N2kDoubleNA);

//*****************************************************************************
// Humidity
// Humidity should be a percent
// Input:
//  - SID                   Sequence ID.
//  - HumidityInstance      This should be unic at least on one device. May be best to have it unic over all devices sending this PGN.
//  - HumiditySource        see tN2kHumiditySource
//  - Humidity              Humidity in percent
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kHumidity(std::vector<std::string> &message,unsigned char SID, unsigned char HumidityInstance,
                           tN2kHumiditySource HumiditySource, double ActualHumidity, double SetHumidity=N2kDoubleNA);

//*****************************************************************************
// Pressure
// Pressures should be in Pascals
// Input:
//  - SID                   Sequence ID.
//  - PressureInstance      This should be unic at least on one device. May be best to have it unic over all devices sending this PGN.
//  - PressureSource        see tN2kPressureSource
//  - Pressure              Pressure in Pascals. Use function mBarToPascal, if you like to use mBar
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kPressure(std::vector<std::string> &message,unsigned char SID, unsigned char PressureInstance,
                           tN2kPressureSource PressureSource, double Pressure);

//*****************************************************************************
// Set pressure
// Pressures should be in Pascals
// Input:
//  - SID                       Sequence ID.
//  - PressureInstance          This should be unic at least on one device. May be best to have it unic over all devices sending this PGN.
//  - PressureSource            see tN2kPressureSource
//  - Set pressure              Set pressure in Pascals. Use function mBarToPascal, if you like to use mBar
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kSetPressure(std::vector<std::string> &message,unsigned char SID, unsigned char PressureInstance,
                              tN2kPressureSource PressureSource, double SetPressure);

//*****************************************************************************
// Temperature
// Temperatures should be in Kelvins
// Input:
//  - SID                   Sequence ID.
//  - TempInstance          This should be unic at least on one device. May be best to have it unic over all devices sending this PGN.
//  - TempSource            see tN2kTempSource
//  - ActualTemperature     Temperature in K. Use function CToKelvin, if you want to use °C.
//  - SetTemperature        Set temperature in K. Use function CToKelvin, if you want to use °C. This is meaningfull for temperatures,
//                          which can be controlled like cabin, freezer, refridgeration temperature. God can use value for this for
//                          outside and sea temperature values.
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kTemperatureExt(std::vector<std::string> &message,unsigned char SID, unsigned char TempInstance, tN2kTempSource TempSource,
                                 double ActualTemperature, double SetTemperature=N2kDoubleNA);


//*****************************************************************************
// Small Craft Status (Trim Tab Position)
// Trim tab position is a percentage 0 to 100% where 0 is fully retracted and 100 is fully extended
// Input:
//  - PortTrimTab           Port trim tab position
//  - StbdTrimTab           Starboard trim tab position

// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kTrimTab(std::vector<std::string> &message,int8_t PortTrimTab, int8_t StbdTrimTab);

#endif //OPEN_BRIDGE_N2KMESSAGE_H
