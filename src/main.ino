#include <Arduino.h>
#include <SoftwareSerial.h>

#include "instruction.h"
#include "utils.h"
#include "motors.h"
#include "distance.h"
#include "encoder.h"

Adafruit_BluefruitLE_SPI ble(4, 19, -1);
Distance dist(9, 10);
Motors mot(6, 8, 5, 7);
Encoder enc;

void setup(void)
{

  //pinMode(5, OUTPUT);
  Serial.begin(115200);
  
  //Serial.begin(115200);
  dist.init();
  mot.init();
  enc.init();
  pinMode(13, OUTPUT);
}

void loop(void) {
  int d = dist.getDist();
  mot.setSpeed(d > 20 ? 200 : -200, 0);
  delay(100);
  ble.print(enc.getDiretion());
}
