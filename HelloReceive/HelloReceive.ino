// Copyright 2015, Matthijs Kooijman <matthijs@stdin.nl>
//
// Permission is hereby granted, free of charge, to anyone
// obtaining a copy of this document and accompanying files, to do
// whatever they want with them without any restriction, including, but
// not limited to, copying, modification and redistribution.
//
// NO WARRANTY OF ANY KIND IS PROVIDED.
//
//
// This example prints any received ZigBee radio packets to serial.

// This example was modified somewhat by Karlduino (https://github.com/karlduino)
// to work with an Arduino Yun. Using a Sparkfun Xbee shield, with pins 0-3 of Yun
// not connected to the shield, and with shield pins 2 & 13 connected, and also
// shield pins 3 and 5 connected (so that 13 is Rx, and 5 is Tx)
//
// Also modified this sketch to use Console for printing debug messages

#include <XBee.h>
#include <Printers.h>
#include <AltSoftSerial.h>
#include <Console.h>

XBeeWithCallbacks xbee;

AltSoftSerial SoftSerial;
#define DebugSerial Console
#define XBeeSerial SoftSerial

#define ALTSS_USE_TIMER3
#define INPUT_CAPTURE_PIN      13 // receive
#define OUTPUT_COMPARE_A_PIN        5 // transmit

void setup() {
  Bridge.begin();
  // Setup debug serial output
  //DebugSerial.begin(115200);
  Console.begin();
  while(!Console);
  DebugSerial.println(F("Starting..."));

  // Setup XBee serial communication
  XBeeSerial.begin(9600);
  xbee.begin(XBeeSerial);
  delay(1);

  // Setup callbacks
  xbee.onZBExplicitRxResponse(processRxPacket);
  xbee.onZBRxResponse(processRxPacket);
}

void processRxPacket(ZBRxResponse& rx, uintptr_t) {
  DebugSerial.print(F("Received packet from "));
  printHex(DebugSerial, rx.getRemoteAddress64());
  DebugSerial.println();
  DebugSerial.print(F("Payload: "));
  DebugSerial.write(rx.getData(), rx.getDataLength());
  DebugSerial.println();
}

void processRxPacket(ZBExplicitRxResponse& rx, uintptr_t) {
  DebugSerial.print(F("Received packet from "));
  printHex(DebugSerial, rx.getRemoteAddress64());
  DebugSerial.println();
  DebugSerial.print(F("Payload: "));
  DebugSerial.write(rx.getData(), rx.getDataLength());
  DebugSerial.println();
}

void loop() {
  // Check the serial port to see if there is a new packet available
  xbee.loop();
}
