#include <Arduino.h>

class Motors {
public:
  Motors(int leftA, int leftB, int rightA, int rightB);

  void init();

  void setSpeed(int left, int right);

private:
  int leftA;
  int leftB;
  int rightA;
  int rightB;
};

Motors::Motors(int leftA, int leftB, int rightA, int rightB)
  : leftA(leftA), leftB(leftB), rightA(rightA), rightB(rightB) {}

void Motors::init() {
  pinMode(leftA, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightA, OUTPUT);
  pinMode(rightB, OUTPUT);
}

void Motors::setSpeed(int left, int right) {
  if (left >= 0) {
    analogWrite(leftA, left);
    digitalWrite(leftB, LOW);
  } else {
    analogWrite(leftA, 255 + left);
    digitalWrite(leftB, HIGH);
  }
  if (right >= 0) {
    analogWrite(rightA, right);
    digitalWrite(rightB, LOW);
  } else {
    analogWrite(rightA, 255 + right);
    digitalWrite(rightB, HIGH);
  }
}
