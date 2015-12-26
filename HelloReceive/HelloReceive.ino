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

#include <XBee.h>
#include <Printers.h>
#include <AltSoftSerial.h>

XBeeWithCallbacks xbee;

AltSoftSerial SoftSerial;
#define DebugSerial Serial
#define XBeeSerial SoftSerial

void setup() {
  // Setup debug serial output
  DebugSerial.begin(115200);
  DebugSerial.println(F("Starting..."));

  // Setup XBee serial communication
  XBeeSerial.begin(9600);
  xbee.begin(XBeeSerial);
  delay(1);

  // Setup callbacks
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

void loop() {
  // Check the serial port to see if there is a new packet available
  xbee.loop();
}
