#include <Arduino.h>

#include "instruction.h"
#include "utils.h"
#include "motors.h"
#include "distance.h"

Bluetooth ble(4, 14, -1);
Distance dist(9, 10);
Motors mot(6, 8, 5, 7);
Instruction receive;
Instruction send;

void print() {
  unsigned int i = mot.getDiretion();
  send = Instruction(Instruction::MISO_DIRECTION, i);
}

void setup(void)
{
  Serial.begin(115200);
  ble.init();
  dist.init();
  mot.setLeftListener(&print);
  mot.setRightListener(&print);
  mot.init();
}

void loop(void) {
  while (ble.available()) {
    printDbg();
    ble.read(receive);
    mot.setSpeed(receive.getData(0), receive.getData(1));
  }
  int di = mot.getDiretion();
  Serial.print("Direc: ");
  Serial.println(di);
  ble.send(Instruction(Instruction::MISO_DIRECTION, di));
  delay(5);
  int d = dist.getDist();
  ble.send(Instruction(Instruction::MISO_DIST, d));
  Serial.print("Dist: ");
  Serial.println(d);
  delay(50);
}
