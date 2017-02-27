#include <Arduino.h>

class Distance {
public:
  Distance(int echo, int trig);
  void init();
  unsigned char getDist();

private:
  int echo;
  int trig;
};

Distance::Distance(int echo, int trig) : echo(echo), trig(trig) {}

void Distance::init() {
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
}

unsigned char Distance::getDist() {
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  int in = pulseIn(echo, HIGH, 10000)/44;
  if (in > 255)
    return 255;
  else
    return in;
}
