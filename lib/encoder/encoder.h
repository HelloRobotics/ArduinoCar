#include <Arduino.h>

class Encoder {
public:
  Encoder();
  static void init();
  unsigned char getDiretion();
  const static int coef = 2;
};

void leftUpdate();
void rightUpdate();
int countLeft;
int countRight;

Encoder::Encoder() {}

void Encoder::init() {
  attachInterrupt(0, leftUpdate, RISING);
  attachInterrupt(1, rightUpdate, RISING);
}

void leftUpdate() {
  countLeft++;
}

void rightUpdate() {
  countRight++;
}


unsigned char Encoder::getDiretion() {
  float f = (countRight - countLeft) * 2.9;
  int angle = f > 0 ? f + 0.5 : f - 0.5;
  if (angle < 0) {
    return (-angle) % 255 + 255;
  } else {
    return angle % 255;
  }
}
