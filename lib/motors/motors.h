#include <Arduino.h>

class Encoder {
public:
  static Encoder* active;

  Encoder();
  unsigned char getDiretion();
  void setRightListener(void (*listener)());
  void setLeftListener(void (*listener)());
  void onLeftUpdate();
  void onRightUpdate();

private:
  bool forwardLeft;
  bool forwardRight;
  int countLeft;
  int countRight;
  void (*leftListener)();
  void (*rightListener)();

  void init();

friend class Motors;
};

void _encoder_leftUpdate();
void _encoder_rightUpdate();

class Motors {
public:
  Motors(int leftA, int leftB, int rightA, int rightB);
  void init();
  void setSpeed(int left, int right);
  void setRightListener(void (*listener)());
  void setLeftListener(void (*listener)());
  unsigned char getDiretion();

private:
  Encoder enc;
  int leftA;
  int leftB;
  int rightA;
  int rightB;
};
