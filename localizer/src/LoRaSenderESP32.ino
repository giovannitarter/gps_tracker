#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {

  delay(2000);
  
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(7, 10, 1); // set CS, reset, IRQ pin

  Serial.println("LoRa Sender");

  if (!LoRa.begin(434E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.dumpRegisters(Serial);
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
