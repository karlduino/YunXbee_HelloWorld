## XBee with Arduino Yun

I'm working through
[Wireless Sensor Networks using Arduino](https://www.packtpub.com/packtlib/book/Hardware%20&%20Creative/9781784395582)
by Matthijs Kooijman, on using Arduinos and XBees.

I'd like to be able to use an
[Arduino Y&uacute;n](https://www.arduino.cc/en/Main/ArduinoBoardYun)
as the main board. I'm thinking that the Y&uacute;n could receive data
from various sensors via the [XBee](http://www.digi.com/lp/xbee), and
then pass it elsewhere (or have the Y&uacute;n be a local web server
with the info).

I'm using a
[Sparkfun XBee shield](https://www.sparkfun.com/products/12847) to
connect Arduinos to XBees. Using this with the Y&uacute;n turns out to
be a bit tricky: see [here](http://bit.ly/yun_xbee) and
[here](http://bit.ly/yun_xbee_2).

This repository concerns the example in Chapter 2 of Kooijman's book,
on sending a "Hello world" message between two XBees. Here modified to
get it to work, particularly with the Y&uacute;n connected to the
coordinator XBee.

### Hardware setup

- Router XBee set as API Router, attached via the Sparkfun XBee shield
  (set at "Dline") to
  [Arduino Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno).
  
- Coordinator XBee set as API Coordinator, attached via Sparkfun XBee
  shield (set at "Dline") to Arduino Y&uacute;n, but with some tricks:
  
  - using an extra set of stacking headers, for clearance to deal with
    the ethernet port on the Y&uacute;n, which is a bit tall
    
  - pins 0,1,2,3 are not connected between the Y&uacute;n and the
    shield (used a 6-pin stacking header for pins 2-7, with 0 and 1
    left open, and then bent out the first two, for pins 2 and 3, so
    that they're not connected)
    
  - jumpers from pin 2 of shield to pin 13, and from pin 3 to pin 5
  
### Software changes

- Using Console for printing debug messages, and so added `#include
  <Console.h>` and changed `Serial` to `Console` in `#define
  DebugSerial Serial`. Also deleted the `DebugSerial.begin(115200)`
  and added `Bridge.begin()`, `Console.begin()`, and
  `while(!Console);`
  
- To use pins 13 and 5 for Rx/Tx with XBee, added:

  ```
  #define ALTSS_USE_TIMER3
  #define INPUT_CAPTURE_PIN      13 // receive
  #define OUTPUT_COMPARE_A_PIN        5 // transmit
  ```

- I needed to make a further change to the `HelloReceive` sketch.
  I originally didn't appear to be receiving any messages, but in
  poking around with
  [XCTU](http://www.digi.com/products/xbee-rf-solutions/xctu-software/xctu),
  I found that the problem was the coordinator Xbee was getting an
  "ExplicitRxResponse" rather than just an "RxResponse". I solved this
  by adding `xbee.onZBExplicitRxResponse(processRxPacket);` plus an
  alternative version of the `processRxPacket` with
  `ZBExplicitRxResponse& rx` as the first input.


### Final notes

Router XBee + Arduino Uno are running the HelloSend sketch, to send a
"Hello, world!" message to the Coordinator XBee every 10 seconds.

Coordinator XBee + Arduino Y&uacute;n are running the HelloReceive
sketch, to print (to Console) what ever the XBee receives.
