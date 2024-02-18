/*
  LoRa register dump

  This examples shows how to inspect and output the LoRa radio's
  registers on the Serial interface
*/
#include <SPI.h>              // include libraries
#include "LoRa-RP2040.h"

void setup() {

  delay(2000);

  Serial.begin(115200);               // initialize serial
  while (!Serial);

    LoRa.setPins(1, 4, 5); // set CS, reset, IRQ pin

  Serial.println("LoRa Dump Registers");

  if (!LoRa.begin(434E6)) {         // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                   // if failed, do nothing
  }

  LoRa.dumpRegisters(Serial);

}


void loop() {


  int packetSize = 0;

  Serial.println("\n\rListening");
  while (true) {
    packetSize = LoRa.parsePacket();
    if (packetSize) {
      break;
    }
  }

  if (packetSize) {
  // received a packet
  Serial.print("Received packet:\n\r\'");

  // read packet
  uint32_t len = 0;
  while (LoRa.available()) {
    Serial.print((char)LoRa.read());
    len += 1;
  }

  // print RSSI of packet
  Serial.printf("\'\n\rRSSI: %d, len: %d\n\r", LoRa.packetRssi(), len);
  }
}
