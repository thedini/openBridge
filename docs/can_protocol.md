# CAN Overview

Controller Area Network has been around since the 1980s and was originally developed by Bosch for automotive applications. Almost every car on the road today utilizes some type of CAN network to assist in controlling various different in the car. From headlights to door locks, the ability to control multiple devices with a single pair of wires has an obvious cost saving benefit. CAN is not fast when compared with TCP/IP (Max of 1Mbps), but it is robust and reliable and designed for real time applications.

The CAN protocol is defined in ISO 11783 and has been extended for various applications, ranging from heavy equipment to factory automation. OpenBridge specifically targets the maritime application with NMEA 2000. The National Marine Electronics Association (NMEA) developed the application layer that sits on top of the [ISO 11783](https://www.iso.org/standard/57556.html) standard. It has defined various Parameter Group Numbers (PGN) that define what data is passed for particular devices. NMEA keeps the official documentation to the standard closed but much of the library has been reverse engineered and can be found in the [CANboat Project](https://github.com/canboat/canboat). Of specific interest is the [.json object](https://github.com/canboat/canboat/blob/master/analyzer/pgns.json) with all PGNs identified to date.

Many tools like CanBoat can decode the PGNs for you but with the goal of learning about CAN and how it can help us monitor and learn about maritime electronics, let's jump into a quick overview of CAN and how to decode PGNs. For more on this, please see the [links](links.md) page for more great resources.

On CAN and NMEA 2000 there is no central controller each node on the network is its own ECU and is responsible to receiving and transmitting in accordance with the standard.

## The Wires

Generally NMEA 2000 and CAN are comprised of two wires, a CAN High (hi) and CAN Low (lo).

![Wikipedia CAN Signaling](https://upload.wikimedia.org/wikipedia/commons/thumb/1/1a/ISO11898-2.svg/1920px-ISO11898-2.svg.png)

As you can see there are two terms here. **Dominant** Voltage, where the signal is moving away from the steady state voltage (in this case 2.5V) and **Recessive** Voltage where the voltage of the High and Low are the same. If no signal is given to the bus it will maintain the recessive state. The recessive state is translated into a binary signal of **1** while the dominant is read as a **0**. This becomes important later when considering arbitration and bit stuffing but for now look at the CAN lo signal and see how it compares with the Driver Logic and know this is what happens on the wire electrically.

Importantly the speed of NMEA 2000 is 250Kbps but the standard allows for high speeds for shorter distances and slower speeds for longer distances.

If you captured the voltages on the CAN hi and CAN Lo of a NMEA 2000 and zoomed into a single frame (A frame being the term for one of four types of can messages) you would get something looking like this:

![CAN Voltages](../img/can_voltage.png)

In this case you can see a little more sloppy version of the neat tidy Can Signal. Both plots are centered around 1.7V but it is not entirely obvious due to scale. You can see that the CAN Lo (the lower line) is getting pulled down while the CAN Hi is getting pulled up. It is difficult to determine but the first three bits of this frame are 001 where the start of frame is identified by the first **Dominant** bit. (If we keep saying it eventually it will make sense!)

We already stated that each node is its own ECU meaning it is responsible for independent operation. To know when to start to listen is important in this case we look a the first Dominant bit as a good place to start. The CAN chips are hard coded in silicone to look for that **Start of Frame** bit (SoF) and begin to listen.

This plot is all fine and good but in reality we need more to determine what we are actually looking at. For this we need to use a tool known as a Logic Analyzer, specifically one that can decode CAN protocol identifiers. The tool I am using is an expensive one, a [Saleae Logic Pro 16](https://www.saleae.com/). They make beautiful software and wonderful hardware, the Logic 8 is much more reasonable and has [student, hacker pricing](https://blog.saleae.com/saleae-discounts/). There are other logic analyzers with different capabilities like the [Analog Discovery 2](https://store.digilentinc.com/analog-discovery-2-100msps-usb-oscilloscope-logic-analyzer-and-variable-power-supply/) which also can be a scope and signal generator but the software is a bit more dated and not as immediately intuitive for hobbyists.

## The Identifier

CAN has a standard 11 bit and extended 29 bit identifier. We mentioned that a node will be listening for a SoF bit, or when the state changes from recessive to dominant. After that our identifier begins. 
