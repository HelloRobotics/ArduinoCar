#include "motors.h"

Encoder* Encoder::active = NULL;

Encoder::Encoder() {}

void Encoder::init() {
  attachInterrupt(0, &_encoder_leftUpdate, CHANGE);
  attachInterrupt(1, &_encoder_rightUpdate, CHANGE);
  Encoder::active = this;
}

void _encoder_leftUpdate() {
  (Encoder::active)->onLeftUpdate();
}

void _encoder_rightUpdate() {
  (Encoder::active)->onRightUpdate();
}

void Encoder::onLeftUpdate() {
  static long lastTime = millis() - 10;
  if (millis() - lastTime > 5) {
    lastTime = millis();
    if (forwardLeft)
      countLeft++;
    else
      countLeft--;
    if (leftListener != NULL) {
      (*leftListener)();
    }
  }
}

void Encoder::onRightUpdate() {
  static long lastTime = millis() - 10;
  if (millis() - lastTime > 5) {
    lastTime = millis();
    if (forwardRight)
      countRight++;
    else
      countRight--;
    if (rightListener != NULL) {
      (*rightListener)();
    }
  }
}

void Encoder::setRightListener(void (*rightListener)()) {
  this->rightListener = rightListener;
}

void Encoder::setLeftListener(void (*leftListener)()) {
  this->leftListener = leftListener;
}

unsigned char Encoder::getDiretion() {
  float f = (countRight - countLeft) * 1.515;
  int angle = f > 0 ? f + 0.5 : f - 0.5;
  if (angle < 0) {
    return (-angle) % 255 + 255;
  } else {
    return angle % 255;
  }
}

Motors::Motors(int leftA, int leftB, int rightA, int rightB)
  : enc(), leftA(leftA), leftB(leftB), rightA(rightA), rightB(rightB) {}

void Motors::init() {
  pinMode(leftA, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightA, OUTPUT);
  pinMode(rightB, OUTPUT);
  enc.init();
}

void Motors::setSpeed(int left, int right) {
  if (left >= 0) {
    analogWrite(leftA, left*2);
    digitalWrite(leftB, LOW);
  } else {
    analogWrite(leftA, 255 + left*2);
    digitalWrite(leftB, HIGH);
  }
  if (right >= 0) {
    analogWrite(rightA, right*2);
    digitalWrite(rightB, LOW);
  } else {
    analogWrite(rightA, 255 + right*2);
    digitalWrite(rightB, HIGH);
  }
  enc.forwardLeft = left >= 0;
  enc.forwardRight = right >= 0;
}

void Motors::setRightListener(void (*listener)()) {
  enc.setRightListener(listener);
}

void Motors::setLeftListener(void (*listener)()) {
  enc.setLeftListener(listener);
}

unsigned char Motors::getDiretion() {
  return enc.getDiretion();
}