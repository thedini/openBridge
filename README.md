# openBridge

A project with the purpose of making, exploring and hacking NMEA2000 and marine electronics and making learning achievable without expensive hardware. The ultimate goal is to enable creation of bridge simulations that can be output onto an actual NMEA 2000 bus to enable exploration of the protocol and real world systems at scale. OpenBridge leverages the hard work of the community around these endeavors and hopes to standardize software and hardware as well as adding detailed documentation to low cost exposure to this technology to the wider maritime, engineering and maker/hacker community.

## The Mission

OpenBridge will attempt to leverage exiting work from [CanBoat](https://github.com/canboat/canboat) and [ttlappalaine NMEA 2000](https://github.com/ttlappalainen/NMEA2000) libraries to create a suite of micro-services (containers) that can serve as maritime electronic devices for learning, testing and simulation. By standardizing the hardware and software and having a focus on documentation OpenBridge hopes to make exploration open to groups that would normally not have access or ability to dive into the world of maritime electronics.

Ideally, each micro-service can act as if it is the authentic electronic device using the Teensy as a gateway to an actual ship or test NMEA 2000 bus. OpenBridge will grow to become a curriculum on NMEA 2000 analysis and shipboard security curriculum to provide the basis for self study or teaching! The hope is to give everyone the ability to explore this interesting niche of technology!

## The Hardware

The hardware is based on the latest Teensy (4.0 and 4.1). The Teensy provides a huge capability for the price which will lend itself to expansion at a later time. The Teensy also has **three** CAN interfaces to enable further expansion. The Teensy will be mounted on breadboard, perf-board or custom PCB that will enable it to easily connect to an NEMA 2000 bus in a standard fashion. The idea of standardizing on hardware is born from the desire to have a standard platform to develop on and support. As can be expected, most of the associated code and procedures can be adapted for various other platform which are outside the scope of this project. (ESP32, Arduino Mega, Raspberry Pi)

The CAN interfaces will require CAN transceivers (VD230) to accommodate conversion from Can High and Can Low to Can TX and Can RX. These device have been chosen based on their general availability and price.

The system can be built on a Breadboard, or utilize custom PCB to accommodate NMEA 2000 plug. In its simplest implementation it requires little more than a Teensy, the VD230 and some hookup wires.

Specific hardware can be found in the [BoM](docs/bom.md) with board designs in [schmatics](schmatics/)

## Software

The software side of the OpenBridge is going to leverage [ttlappalaine NMEA 2000](https://github.com/ttlappalainen/NMEA2000) and [ttlappalaine's Teeensy Library](https://github.com/ttlappalainen/NMEA2000_Teensyx) as a base. Code is stored in the `src` directory and organized by function. Follow the `docs` to utilize open source code and open hardware to study NMEA2000.

In its most simple implementation you can use OpenBridge to start looking at data with one of many Logic Analyzers and learning how to decipher different parts of the NMEA2000 CAN Frame with the basic **wind sensor**

Extend the capability of the **wind sensor** by feeding your own wind parameters via a csv file.
