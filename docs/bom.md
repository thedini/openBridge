# Open Bridge BOM

## Teensy

OpenBridge uses a Teensy 4.X as as basis. This devices was choose because if the high number of CAN interfaces as well as lost cost and compatibility with the Arduino ecosystem. The platform should be able to allow for future development of features and capabilities.

- [Teensy 4.1 (26 USD)](https://www.pjrc.com/store/teensy41.html)
- [Teensy 4.0 (20 USD)](https://www.pjrc.com/store/teensy40.html)

## Transceiver

In order to use the Teensy can interfaces you need to change the CAN High and CAN Low signals into a bit stream refrencing 0V or Ground. To do this it is very easy to use a VP230 CAN Transciever. It will pretty simply provide this interface for you.

- [VP 230 Transciever Data Sheet](https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf?ts=1626309738616&ref_url=https%253A%252F%252Fwww.google.ca%252F)
- [VP 230 Breakout on Amazon 5 pack (11 USD)](https://www.amazon.com/ACEIRMC-SN65HVD230-Transceiver-Communication-Protection/dp/B08ZYB9Y6Y)

## NMEA 2000 STUFF

If you want to interface with a NMEA 2000 backbone you will need a few things. These are no 100% necessary but help a lot to make your own home lab or to connect up to a vessel. I usually use one NMEA 2000 Drop Cable that I strip the end of to interface with a proto-board. This is purely because the NMEA 2000 connector alone is like $30. That being said, you may want two drop cables.

- [NMEA 2000 Drop Cable (19 USD)](https://www.amazon.com/Backbone-Lowrance-Simrad-Navico-Networks/dp/B0915GGVH8)
- [NMEA 2000 backbone Connector(17 USD)](https://www.amazon.com/Micro-C-T-Connector-Garmin-Lowrance-Networks/dp/B086N5H6NH)
- [NMEA 2000 Resistors (20 USD)](https://www.amazon.com/Female-Terminators-Terminating-Lowrance-Networks/dp/B086VCRBXP)

## Odds and ends

As you can imagine you need something to connect everything together with! Much of this can be substituted and adapted but:

- [Proto Board](https://www.amazon.com/ElectroCookie-Solderable-Breadboards-Gold-Plated-Multi-Pack/dp/B07ZV9JQY2)
- [Male/Female Headers](https://www.amazon.com/CenryKay-Connectors-Compatible-Arduino-Prototype/dp/B07RRPYZCT)
- [Screw Terminal](https://www.amazon.com/Hxchen-4-Pin-2-54mm-Terminal-Connector/dp/B07WR5NMWN)
- Wire
- Solder
- Soldering Iron
- [12 Volt Power Supply](https://www.electronics-tutorials.ws/blog/convert-atx-psu-to-bench-supply.html) -- An old computer power supply could work
