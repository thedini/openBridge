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


//#include "N2kTypes.h"
#include "N2kmessage.h"
//#include <stdint.h>
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

void SetN2kSystemTime(std::vector<std::string> &message,unsigned char SID, uint16_t SystemDate, double SystemTime, tN2kTimeSource TimeSource){
    int UID = 0;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(SystemDate));
    message.push_back(std::to_string(SystemTime));
    message.push_back(std::to_string(TimeSource));
}

//*****************************************************************************
// Rudder
// Input:
// - RudderPosition         Current rudder postion in radians.
// - Instance               Rudder instance.
// - RudderDirectionOrder   See tN2kRudderDirectionOrder. Direction, where rudder should be turned.
// - AngleOrder             In radians angle where rudder should be turned.
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kRudder(std::vector<std::string> &message,double RudderPosition, unsigned char Instance, tN2kRudderDirectionOrder RudderDirectionOrder, double AngleOrder){
    int UID = 1;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(RudderPosition));
    message.push_back(std::to_string(Instance));
    message.push_back(std::to_string(RudderDirectionOrder));
    message.push_back(std::to_string(AngleOrder));
}


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

void SetN2kTrueHeading(std::vector<std::string> &message,unsigned char SID, double Heading, double Deviation, double Variation){
    int UID = 2;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(Heading));
    message.push_back(std::to_string(Deviation));
    message.push_back(std::to_string(Variation));
}


//*****************************************************************************
// Rate of Turn
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and heading at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - Rate of turn          Change in heading in radians per second
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kRateOfTurn(std::vector<std::string> &message,unsigned char SID, double RateOfTurn){
    int UID = 3;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(RateOfTurn));
}


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
void SetN2kAttitude(std::vector<std::string> &message,unsigned char SID, double Yaw, double Pitch, double Roll){
    int UID = 4;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(Yaw));
    message.push_back(std::to_string(Pitch));
    message.push_back(std::to_string(Roll));
}

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
void SetN2kMagneticVariation(std::vector<std::string> &message,unsigned char SID, tN2kMagneticVariation Source, uint16_t DaysSince1970, double Variation){
    int UID = 5;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(Source));
    message.push_back(std::to_string(DaysSince1970));
    message.push_back(std::to_string(Variation));
}

//*****************************************************************************
// Engine parameters rapid
// Input:
//  - EngineInstance        Engine instance.
//  - EngineSpeed           RPM (Revolutions Per Minute)
//  - EngineBoostPressure   in Pascal
//  - EngineTiltTrim        in %
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kEngineParamRapid(std::vector<std::string> &message,unsigned char EngineInstance, double EngineSpeed, double EngineBoostPressure, int8_t EngineTiltTrim){
    int UID = 6;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(EngineInstance));
    message.push_back(std::to_string(EngineSpeed));
    message.push_back(std::to_string(EngineBoostPressure));
    message.push_back(std::to_string(EngineTiltTrim));
}

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
                              double FuelRate, double EngineHours, double EngineCoolantPress, double EngineFuelPress,
                              int8_t EngineLoad, int8_t EngineTorque, tN2kEngineDiscreteStatus1 Status1, tN2kEngineDiscreteStatus2 Status2){
    int UID = 7;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(EngineInstance));
    message.push_back(std::to_string(EngineOilPress));
    message.push_back(std::to_string(EngineOilTemp));
    message.push_back(std::to_string(EngineCoolantTemp));
    message.push_back(std::to_string(AltenatorVoltage));
    message.push_back(std::to_string(FuelRate));
    message.push_back(std::to_string(EngineHours));
    message.push_back(std::to_string(EngineCoolantPress));
    message.push_back(std::to_string(EngineFuelPress));
    message.push_back(std::to_string(EngineLoad));
    message.push_back(std::to_string(EngineTorque));
    //TODO: Add elemenst of struct
    //message.push_back(std::to_string(Status1));
    //message.push_back(std::to_string(Status2));
}
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
void SetN2kTransmissionParameters(std::vector<std::string> &message,unsigned char EngineInstance, tN2kTransmissionGear TransmissionGear, double OilPressure, double OilTemperature, unsigned char DiscreteStatus1){
    int UID = 8;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(EngineInstance));
    message.push_back(std::to_string(TransmissionGear));
    message.push_back(std::to_string(OilPressure));
    message.push_back(std::to_string(OilTemperature));
    message.push_back(std::to_string(DiscreteStatus1));
}

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
                                double FuelRateAverage, double FuelRateEconomy, double InstantaneousFuelEconomy){
    int UID = 9;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(EngineInstance));
    message.push_back(std::to_string(TripFuelUsed));
    message.push_back(std::to_string(FuelRateAverage));
    message.push_back(std::to_string(FuelRateEconomy));
    message.push_back(std::to_string(InstantaneousFuelEconomy));

}
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
void SetN2kFluidLevel(std::vector<std::string> &message,unsigned char Instance, tN2kFluidType FluidType, double Level, double Capacity){
    int UID = 10;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(Instance));
    message.push_back(std::to_string(FluidType));
    message.push_back(std::to_string(Level));
    message.push_back(std::to_string(Capacity));
}

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
                    unsigned char StateOfCharge, unsigned char StateOfHealth, double TimeRemaining, double RippleVoltage, double Capacity){
    int UID = 11;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(DCInstance));
    message.push_back(std::to_string(DCType));
    message.push_back(std::to_string(StateOfCharge));
    message.push_back(std::to_string(StateOfHealth));
    message.push_back(std::to_string(TimeRemaining));
    message.push_back(std::to_string(RippleVoltage));
    message.push_back(std::to_string(Capacity));
}

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
                         tN2kChargeState ChargeState, tN2kChargerMode ChargerMode,
                         tN2kOnOff Enabled, tN2kOnOff EqualizationPending, double EqualizationTimeRemaining){
    int UID = 12;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(Instance));
    message.push_back(std::to_string(BatteryInstance));
    message.push_back(std::to_string(ChargeState));
    message.push_back(std::to_string(ChargerMode));
    message.push_back(std::to_string(Enabled));
    message.push_back(std::to_string(EqualizationPending));
    message.push_back(std::to_string(EqualizationTimeRemaining));
}

//*****************************************************************************
// Battery Status
// Input:
//  - BatteryInstance       BatteryInstance.
//  - BatteryVoltage        Battery voltage in V
//  - BatteryCurrent        Current in A
//  - BatteryTemperature    Battery temperature in K. Use function CToKelvin, if you want to use °C.
//  - SID                   Sequence ID.
void SetN2kDCBatStatus(std::vector<std::string> &message,unsigned char BatteryInstance, double BatteryVoltage, double BatteryCurrent,
                        double BatteryTemperature, unsigned char SID){
    int UID = 13;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(BatteryInstance));
    message.push_back(std::to_string(BatteryVoltage));
    message.push_back(std::to_string(BatteryCurrent));
    message.push_back(std::to_string(BatteryTemperature));
    message.push_back(std::to_string(SID));
}


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
                   double PeukertExponent, int8_t ChargeEfficiencyFactor){
    int UID = 14;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(BatInstance));
    message.push_back(std::to_string(BatType));
    message.push_back(std::to_string(SupportsEqual));
    message.push_back(std::to_string(BatNominalVoltage));
    message.push_back(std::to_string(BatChemistry));
    message.push_back(std::to_string(BatCapacity));
    message.push_back(std::to_string(BatTemperatureCoefficient));
    message.push_back(std::to_string(PeukertExponent));
    message.push_back(std::to_string(ChargeEfficiencyFactor));
}

//*****************************************************************************
// Leeway
// Input:
//  - SID            Sequence ID field
//  - Leeway         Nautical Leeway Angle, which is defined as the angle between the vessel’s heading (std::vector<std::string> &message,direction to which the
//                   vessel’s bow points) and its course (direction of its motion (track) through the water)
// Output:
//  - N2kMsg         NMEA2000 message ready to be send.
void SetN2kLeeway(std::vector<std::string> &message,unsigned char SID, double Leeway){
    int UID = 15;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(Leeway));
}

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
void SetN2kBoatSpeed(std::vector<std::string> &message,unsigned char SID, double WaterReferenced, double GroundReferenced, tN2kSpeedWaterReferenceType SWRT){
    int UID = 16;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(WaterReferenced));
    message.push_back(std::to_string(GroundReferenced));
    message.push_back(std::to_string(SWRT));

}

//*****************************************************************************
// Water depth
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and depth at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - DepthBelowTransducer  Depth below transducer in meters
//  - Offset                Distance in meters between transducer and surface (positive) or transducer and keel (negative)
//  - Range                 Measuring range
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kWaterDepth(std::vector<std::string> &message,unsigned char SID, double DepthBelowTransducer, double Offset, double Range){
    int UID = 17;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(DepthBelowTransducer));
    message.push_back(std::to_string(Offset));
    message.push_back(std::to_string(Range));
}

//*****************************************************************************
// Distance log
// Input:
//  - DaysSince1970         Timestamp
//  - SecondsSinceMidnight  Timestamp
//  - Log                   Total meters travelled
//  - Trip Log              Meters travelled since last reset
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kDistanceLog(std::vector<std::string> &message,uint16_t DaysSince1970, double SecondsSinceMidnight, uint32_t Log, uint32_t TripLog){
    int UID = 18;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(DaysSince1970));
    message.push_back(std::to_string(SecondsSinceMidnight));
    message.push_back(std::to_string(Log));
    message.push_back(std::to_string(TripLog));

}



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
        tN2kSpeedType SpeedControlType,
        tN2kGenericStatusPair AnchorDockingControl,
        tN2kGenericStatusPair PowerEnable,
        tN2kGenericStatusPair MechanicalLock,
        tN2kGenericStatusPair DeckAndAnchorWash,
        tN2kGenericStatusPair AnchorLight,
        double CommandTimeout,
        const tN2kWindlassControlEvents &WindlassControlEvents){
    int UID = 19;
    message.push_back(std::to_string(UID));

    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(WindlassIdentifier));
    message.push_back(std::to_string(WindlassDirectionControl));
    message.push_back(std::to_string(SpeedControl));
    message.push_back(std::to_string(SpeedControlType));
    message.push_back(std::to_string(AnchorDockingControl));
    message.push_back(std::to_string(PowerEnable));
    message.push_back(std::to_string(MechanicalLock));
    message.push_back(std::to_string(DeckAndAnchorWash));
    message.push_back(std::to_string(AnchorLight));
    message.push_back(std::to_string(CommandTimeout));

    //TODO: Add elemenst of struct
    //message.push_back(std::to_string(WindlassControlEvents));

}


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
        double WindlassLineSpeed,
        tN2kWindlassMotionStates WindlassMotionStatus,
        tN2kRodeTypeStates RodeTypeStatus,
        tN2kDD482 AnchorDockingStatus,
        const tN2kWindlassOperatingEvents &WindlassOperatingEvents){
    int UID = 20;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(WindlassIdentifier));
    message.push_back(std::to_string(RodeCounterValue));
    message.push_back(std::to_string(WindlassLineSpeed));
    message.push_back(std::to_string(WindlassMotionStatus));
    message.push_back(std::to_string(RodeTypeStatus));
    message.push_back(std::to_string(AnchorDockingStatus));

    //TODO: Add elemenst of struct
    //message.push_back(std::to_string(WindlassOperatingEvents));

}


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
        double ControllerVoltage,
        double MotorCurrent,
        const tN2kWindlassMonitoringEvents &WindlassMonitoringEvents){
    int UID = 21;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(WindlassIdentifier));
    message.push_back(std::to_string(TotalMotorTime));
    message.push_back(std::to_string(ControllerVoltage));
    message.push_back(std::to_string(MotorCurrent));

    //TODO: Add elemenst of struct
    //message.push_back(std::to_string(WindlassMonitoringEvents));

}


//*****************************************************************************
// Lat/lon rapid
// Input:
//  - Latitude               Latitude in degrees
//  - Longitude              Longitude in degrees
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kLatLonRapid(std::vector<std::string> &message,double Latitude, double Longitude){
    int UID = 22;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(Latitude));
    message.push_back(std::to_string(Longitude));

}

//*****************************************************************************
// COG SOG rapid
// Input:
//  - COG                   Cource Over Ground in radians
//  - SOG                   Speed Over Ground in m/s
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kCOGSOGRapid(std::vector<std::string> &message,unsigned char SID, tN2kHeadingReference ref, double COG, double SOG){
    int UID = 23;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(ref));
    message.push_back(std::to_string(COG));
    message.push_back(std::to_string(SOG));

}


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
                unsigned char nSatellites, double HDOP, double PDOP, double GeoidalSeparation,
                unsigned char nReferenceStations, tN2kGNSStype ReferenceStationType, uint16_t ReferenceStationID,
                double AgeOfCorrection){
    int UID = 24;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(DaysSince1970));
    message.push_back(std::to_string(SecondsSinceMidnight));
    message.push_back(std::to_string(Latitude));
    message.push_back(std::to_string(Longitude));
    message.push_back(std::to_string(Altitude));
    message.push_back(std::to_string(GNSStype));
    message.push_back(std::to_string(GNSSmethod));
    message.push_back(std::to_string(nSatellites));
    message.push_back(std::to_string(HDOP));
    message.push_back(std::to_string(PDOP));
    message.push_back(std::to_string(GeoidalSeparation));
    message.push_back(std::to_string(nReferenceStations));
    message.push_back(std::to_string(ReferenceStationType));
    message.push_back(std::to_string(ReferenceStationID));
    message.push_back(std::to_string(AgeOfCorrection));

}

//*****************************************************************************
// Date,Time & Local offset  ( see also PGN 126992 )
// Input:
//  - DaysSince1970         Days since 1.1.1970. You can find sample how to convert e.g. NMEA0183 date info to this on my NMEA0183 library on
//                          NMEA0183Messages.cpp on function NMEA0183ParseRMC_nc
//  - Time                  Seconds since midnight
//  - Local offset          Local offset in minutes
void SetN2kLocalOffset(std::vector<std::string> &message,uint16_t DaysSince1970, double SecondsSinceMidnight, int16_t LocalOffset){
    int UID = 25;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(DaysSince1970));
    message.push_back(std::to_string(SecondsSinceMidnight));
    message.push_back(std::to_string(LocalOffset));

}

//*****************************************************************************
// AIS position reports for Class A
// Input:
//  - N2kMsg                NMEA2000 message to decode
void SetN2kAISClassAPosition(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID, double Latitude, double Longitude,
                                bool Accuracy, bool RAIM, uint8_t Seconds, double COG, double SOG, double Heading, double ROT, tN2kAISNavStatus NavStatus){
    int UID = 26;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(MessageID));
    message.push_back(std::to_string(Repeat));
    message.push_back(std::to_string(UserID));
    message.push_back(std::to_string(Latitude));
    message.push_back(std::to_string(Longitude));
    message.push_back(std::to_string(Accuracy));
    message.push_back(std::to_string(RAIM));
    message.push_back(std::to_string(Seconds));
    message.push_back(std::to_string(COG));
    message.push_back(std::to_string(SOG));
    message.push_back(std::to_string(Heading));
    message.push_back(std::to_string(ROT));
    message.push_back(std::to_string(NavStatus));

}

//*****************************************************************************
// AIS position reports for Class B
// Input:
//  - N2kMsg                NMEA2000 message to decode
void SetN2kAISClassBPosition(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID,
                                double Latitude, double Longitude, bool Accuracy, bool RAIM,
                                uint8_t Seconds, double COG, double SOG, double Heading, tN2kAISUnit Unit,
                                bool Display, bool DSC, bool Band, bool Msg22, tN2kAISMode Mode, bool State){
    int UID = 27;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(MessageID));
    message.push_back(std::to_string(Repeat));
    message.push_back(std::to_string(UserID));
    message.push_back(std::to_string(Latitude));
    message.push_back(std::to_string(Longitude));
    message.push_back(std::to_string(Accuracy));
    message.push_back(std::to_string(RAIM));
    message.push_back(std::to_string(Seconds));
    message.push_back(std::to_string(COG));
    message.push_back(std::to_string(SOG));
    message.push_back(std::to_string(Heading));
    message.push_back(std::to_string(Unit));
    message.push_back(std::to_string(Display));
    message.push_back(std::to_string(DSC));
    message.push_back(std::to_string(Band));
    message.push_back(std::to_string(Msg22));
    message.push_back(std::to_string(Mode));
    message.push_back(std::to_string(State));

}

//*****************************************************************************
// Cross Track Error
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kXTE(std::vector<std::string> &message,unsigned char SID, tN2kXTEMode XTEMode, bool NavigationTerminated, double XTE){
    int UID = 28;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(XTEMode));
    message.push_back(std::to_string(NavigationTerminated));
    message.push_back(std::to_string(XTE));

}

//*****************************************************************************
// Navigation info
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kNavigationInfo(std::vector<std::string> &message,unsigned char SID, double DistanceToWaypoint, tN2kHeadingReference BearingReference,
                            bool PerpendicularCrossed, bool ArrivalCircleEntered, tN2kDistanceCalculationType CalculationType,
                            double ETATime, int16_t ETADate, double BearingOriginToDestinationWaypoint, double BearingPositionToDestinationWaypoint,
                            uint8_t OriginWaypointNumber, uint8_t DestinationWaypointNumber,
                            double DestinationLatitude, double DestinationLongitude, double WaypointClosingVelocity){
    int UID = 29;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(DistanceToWaypoint));
    message.push_back(std::to_string(BearingReference));
    message.push_back(std::to_string(PerpendicularCrossed));
    message.push_back(std::to_string(ArrivalCircleEntered));
    message.push_back(std::to_string(CalculationType));
    message.push_back(std::to_string(ETATime));
    message.push_back(std::to_string(ETADate));
    message.push_back(std::to_string(BearingOriginToDestinationWaypoint));
    message.push_back(std::to_string(BearingPositionToDestinationWaypoint));
    message.push_back(std::to_string(OriginWaypointNumber));
    message.push_back(std::to_string(DestinationWaypointNumber));
    message.push_back(std::to_string(DestinationLatitude));
    message.push_back(std::to_string(DestinationLongitude));
    message.push_back(std::to_string(WaypointClosingVelocity));


}
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
                     bool NavDirection, bool SupplementaryData, char* RouteName){
    int UID = 30;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(Start));
    message.push_back(std::to_string(Database));
    message.push_back(std::to_string(Route));
    message.push_back(std::to_string(NavDirection));
    message.push_back(std::to_string(SupplementaryData));
    message.push_back(std::string(RouteName));

}

bool AppendN2kPGN129285(std::vector<std::string> &message,uint16_t WPID2, char* WPName2, double Latitude2, double Longitude2){
    int UID = 31;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(WPID2));
    message.push_back(std::string(WPName2));
    message.push_back(std::to_string(Latitude2));
    message.push_back(std::to_string(Longitude2));
    return true;
}

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
                        double HDOP, double VDOP, double TDOP){
    int UID = 32;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(DesiredMode));
    message.push_back(std::to_string(ActualMode));
    message.push_back(std::to_string(HDOP));
    message.push_back(std::to_string(VDOP));
    message.push_back(std::to_string(TDOP));

}

//*****************************************************************************
//struct tSatelliteInfo {
//    unsigned char PRN;
//    double Elevation;
//    double Azimuth;
//    double SNR;
//    double RangeResiduals;
//    tN2kPRNUsageStatus UsageStatus;
//};

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
void SetN2kPGN129540(std::vector<std::string> &message,unsigned char SID, tN2kRangeResidualMode Mode){
    int UID = 33;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(Mode));

}

// Append new satellite info for GNSS Satellites in View message
// Input:
//  - SatelliteInfo         Requested satellite info.
//
// Return:
//   true  - if function succeeds.
//   false - if no more satellites can be added or try to use wrong or uninitialized message.
void SetN2kGNSSSatellitesInView(std::vector<std::string> &message,unsigned char SID, tN2kRangeResidualMode Mode){
    int UID = 34;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(Mode));

}
bool AppendSatelliteInfo(std::vector<std::string> &message,const tSatelliteInfo& SatelliteInfo){
    int UID = 35;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SatelliteInfo.PRN));
    message.push_back(std::to_string(SatelliteInfo.Elevation));
    message.push_back(std::to_string(SatelliteInfo.Azimuth));
    message.push_back(std::to_string(SatelliteInfo.SNR));
    message.push_back(std::to_string(SatelliteInfo.RangeResiduals));
    message.push_back(std::to_string(SatelliteInfo.UsageStatus));
    return true;
}


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
                            tN2kAISDTE DTE, tN2kAISTranceiverInfo AISinfo){
    int UID = 36;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(MessageID));
    message.push_back(std::to_string(Repeat));
    message.push_back(std::to_string(UserID));
    message.push_back(std::to_string(IMOnumber));
    message.push_back(std::string(Callsign));
    message.push_back(std::string(Name));
    message.push_back(std::to_string(VesselType));
    message.push_back(std::to_string(Length));
    message.push_back(std::to_string(Beam));
    message.push_back(std::to_string(PosRefStbd));
    message.push_back(std::to_string(PosRefBow));
    message.push_back(std::to_string(ETAdate));
    message.push_back(std::to_string(ETAtime));
    message.push_back(std::to_string(Draught));
    message.push_back(std::string(Destination));
    message.push_back(std::to_string(AISversion));
    message.push_back(std::to_string(GNSStype));
    message.push_back(std::to_string(DTE));
    message.push_back(std::to_string(AISinfo));

}


//*****************************************************************************
// AIS static data class B part A
// Input:
//  - MessageID             Message type
//  - Repeat                Repeat indicator
//  - UserID                MMSI
//  - Name                  Vessel name
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kAISClassBStaticPartA(std::vector<std::string> &message,uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID, char *Name){
    int UID = 37;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(MessageID));
    message.push_back(std::to_string(Repeat));
    message.push_back(std::to_string(UserID));
    message.push_back(std::string(Name));

}

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
                                double PosRefStbd, double PosRefBow, uint32_t MothershipID){
    int UID = 38;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(MessageID));
    message.push_back(std::to_string(Repeat));
    message.push_back(std::to_string(UserID));
    message.push_back(std::to_string(VesselType));
    message.push_back(std::string(Vendor));
    message.push_back(std::string(Callsign));
    message.push_back(std::to_string(Length));
    message.push_back(std::to_string(Beam));
    message.push_back(std::to_string(PosRefStbd));
    message.push_back(std::to_string(PosRefBow));
    message.push_back(std::to_string(MothershipID));

}

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
void SetN2kWaypointList(std::vector<std::string> &message,uint16_t Start, uint16_t NumWaypoints, uint16_t Database){
    int UID = 39;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(Start));
    message.push_back(std::to_string(NumWaypoints));
    message.push_back(std::to_string(Database));

}

//*****************************************************************************
// Wind Speed
// Input:
//  - SID                   Sequence ID. If your device is e.g. boat speed and wind at same time, you can set same SID for different messages
//                          to indicate that they are measured at same time.
//  - WindSpeed             Measured wind speed in m/s
//  - WindAngle             Measured wind angle in radians. If you have value in degrees, use function DegToRad(std::vector<std::string> &message,myval) in call.
//  - WindReference         Wind reference, see definition of tN2kWindReference
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kWindSpeed(std::vector<std::string> &message,unsigned char SID, double WindSpeed, double WindAngle, int WindReference){
    int UID = 40;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(WindSpeed));
    message.push_back(std::to_string(WindAngle));
    message.push_back(std::to_string(WindReference));

}

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
                                            double OutsideAmbientAirTemperature, double AtmosphericPressure){
    int UID = 41;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(WaterTemperature));
    message.push_back(std::to_string(OutsideAmbientAirTemperature));
    message.push_back(std::to_string(AtmosphericPressure));

}

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
                                    tN2kHumiditySource HumiditySource, double Humidity, double AtmosphericPressure){
    int UID = 42;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(TempSource));
    message.push_back(std::to_string(Temperature));
    message.push_back(std::to_string(HumiditySource));
    message.push_back(std::to_string(Humidity));
    message.push_back(std::to_string(AtmosphericPressure));

}

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
                        double ActualTemperature, double SetTemperature){
    int UID = 43;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(TempInstance));
    message.push_back(std::to_string(TempSource));
    message.push_back(std::to_string(ActualTemperature));
    message.push_back(std::to_string(SetTemperature));

}

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
                    tN2kHumiditySource HumiditySource, double ActualHumidity, double SetHumidity){
    int UID = 44;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(HumidityInstance));
    message.push_back(std::to_string(HumiditySource));
    message.push_back(std::to_string(ActualHumidity));
    message.push_back(std::to_string(SetHumidity));

}

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
                    tN2kPressureSource PressureSource, double Pressure){
    int UID = 45;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(PressureInstance));
    message.push_back(std::to_string(PressureSource));
    message.push_back(std::to_string(Pressure));

}

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
                        tN2kPressureSource PressureSource, double SetPressure){
    int UID = 46;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(PressureInstance));
    message.push_back(std::to_string(PressureSource));
    message.push_back(std::to_string(SetPressure));

}

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
                            double ActualTemperature, double SetTemperature){
    int UID = 47;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(SID));
    message.push_back(std::to_string(TempInstance));
    message.push_back(std::to_string(TempSource));
    message.push_back(std::to_string(ActualTemperature));
    message.push_back(std::to_string(SetTemperature));

}

//*****************************************************************************
// Small Craft Status (Trim Tab Position)
// Trim tab position is a percentage 0 to 100% where 0 is fully retracted and 100 is fully extended
// Input:
//  - PortTrimTab           Port trim tab position
//  - StbdTrimTab           Starboard trim tab position

// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
void SetN2kTrimTab(std::vector<std::string> &message,int8_t PortTrimTab, int8_t StbdTrimTab){
    int UID = 48;
    message.push_back(std::to_string(UID));
    message.push_back(std::to_string(PortTrimTab));
    message.push_back(std::to_string(StbdTrimTab));

}