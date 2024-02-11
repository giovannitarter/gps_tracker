#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS.h>

int counter = 0;

TinyGPS gps;

void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial1.begin(9600, SERIAL_8N1, 3, 2);
  while (!Serial1);

  LoRa.setPins(7, 10, 1); // set CS, reset, IRQ pin

  Serial.println("\n\n\n\rLoRa Sender");

  if (!LoRa.begin(434E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("Dump Register");
  LoRa.dumpRegisters(Serial);

}

void loop() {

  Serial.printf("\n\r");
  Serial.printf("uptime: %d\n\r", millis());

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  while (Serial1.available() > 0) {
    if (gps.encode(Serial1.read())) {
      newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.print(' ');
  }

  gps.stats(&chars, &sentences, &failed);
  Serial.print("CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0) {
    Serial.println("** No characters received from GPS: check wiring **");
  }


  Serial.printf("Sending packet: %d\n\r", counter);

  // send packet
  LoRa.beginPacket();
  LoRa.printf("% 10d - hello %d", millis(), counter);
  //LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
