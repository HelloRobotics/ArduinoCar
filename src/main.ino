#include <Arduino.h>

#include "instruction.h"
#include "utils.h"
#include "motors.h"
#include "distance.h"
#include "encoder.h"

Bluetooth ble(4, 14, -1);
Distance dist(9, 10);
Motors mot(6, 8, 5, 7);
Encoder enc;
Instruction ins;

void setup(void)
{
  Serial.begin(115200);
  ble.init();
  dist.init();
  mot.init();
  enc.init();
  pinMode(13, OUTPUT);
}

void loop(void) {
  if (ble.available()) {
    ble.read(ins);
    Serial.print((int)ins.getData(0));
    mot.setSpeed(ins.getData(0), ins.getData(1));
    ble.send(Instruction(Instruction::MISO_TEXT, "Hi!", 4));
  }
}
