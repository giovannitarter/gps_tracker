#include <SPI.h>
#include <LoRa.h>
#include <TinyGPSPlus.h>

int counter = 0;

uint32_t time_to_time = 0;
uint32_t time_to_pos = 0;

TinyGPSPlus gps;


void writeInfo(char *out, size_t len)
{

  char location[30];
  if (gps.location.isValid()) {

    if (time_to_pos == 0) {
      time_to_pos = millis();
    }

    snprintf(location, 30, "%.6f %.6f", gps.location.lat(), gps.location.lng());
  }
  else {
    strcpy(location, "INVALID");
  }

  if (time_to_time == 0) {
    if (gps.date.year() != 2000) {
      time_to_time = millis();
    }
  }

  snprintf(out, len,
    "Location: %s, Date/Time: %02d/%02d/%04d %02d:%02d:%02d.%02d",
    location,
    gps.date.day(),
    gps.date.month(),
    gps.date.year(),
    gps.time.hour(),
    gps.time.minute(),
    gps.time.second(),
    gps.time.centisecond()
    );

}

void setup() {

  Serial.begin(115200);
  //while (!Serial);

  Serial1.begin(9600, SERIAL_8N1, 3, 2);
  //while (!Serial1);

  delay(1000);

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
  char position[100];

  while (Serial1.available() > 0) {
    if (gps.encode(Serial1.read())) {
    }
  }

  writeInfo(position, 100);
  Serial.println(position);

  Serial.printf("Sending packet: %d\n\r", counter);

  // send packet
  LoRa.beginPacket();
  LoRa.printf("% 10d - %s %d %d %d",
    millis(),
    position,
    counter,
    time_to_time,
    time_to_pos
    );
  LoRa.endPacket();

  counter++;

  delay(5000);
}
