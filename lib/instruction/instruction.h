#ifndef HELLOROBOTICS_INSTRUCTION_H
#define HELLOROBOTICS_INSTRUCTION_H

#include "Adafruit_BluefruitLE_SPI.h"
#include <SoftwareSerial.h>

#include "utils.h"

class Instruction {
public:
    const static char MOSI_TEXT = 0x00;
    const static char MOSI_REQUEST = 0x01;
    const static char MOSI_SPEED = 0x02;
    const static char MISO_TEXT = 0x00;
    const static char MISO_DIST = 0x01;
    const static char MISO_DIRECTION = 0x02;
    const static char INST_ERROR = (const char) -1;
    const static size_t LENGTH = 18;
    const static size_t LENGTH_UNDEFINED = (const size_t) -1;
    const static size_t LENGTH_DYNAMIC = (const size_t) -2;
    const static char ETB = 0x17;

    Instruction();
    Instruction(char code, char data);
    Instruction(char code, char dataA, char dataB);
    Instruction(char code, const char data[], size_t size);

    char* toBuf() const;
    bool fromBuf(const char* buf);
    void clear();
    bool isValid() const;
    bool isFinished() const;
    char* getData() const;
    char getData(size_t index) const;
    char getCode() const;

    static size_t codeSize(const char code,  bool mosi);

private:
    static char buf[LENGTH+2];

    bool mosi;
    char code;
    char data[LENGTH];

    static void markEnd();
};

char Instruction::buf[LENGTH+2] = {};

size_t Instruction::codeSize(const char code, bool mosi) {
    if (mosi) {
        switch (code) {
            case MOSI_TEXT:
            case MOSI_REQUEST:
                return LENGTH_DYNAMIC;
            case MOSI_SPEED:
                return 2;
            default:
                return LENGTH_UNDEFINED;
        }
    } else {
        switch (code) {
            case MISO_TEXT:
                return LENGTH_DYNAMIC;
            case MISO_DIST:
            case MISO_DIRECTION:
                return 1;
            default:
                return LENGTH_UNDEFINED;
        }
    }
}

Instruction::Instruction() : code(INST_ERROR) {};

Instruction::Instruction(char code, char data) :
        mosi(false), code(code), data{data} {
    if (codeSize(code, false) != 1) {
        clear();
    }
}

Instruction::Instruction(char code, char dataA, char dataB) :
        mosi(false), code(code), data{dataA, dataB} {
    if (codeSize(code, false) != 2) {
        clear();
    }
}

Instruction::Instruction(char code, const char data[], size_t size) :
        mosi(false), code(code) {
    if (size > LENGTH || codeSize(code, false) != LENGTH_DYNAMIC)
        clear();
    else
        memcpy(this->data, data, size);
}

char* Instruction::toBuf() const {
    *buf = code;
    memcpy(buf + 1, data, LENGTH);
    if (!mosi) {
        switch (code) {
            case MISO_TEXT:
                markEnd(); break;
            case MISO_DIRECTION:
            case MISO_DIST:
                buf[2] = ETB; break;
            default:
                buf[1] = ETB; break;
        }
    }
    return buf;
}

void Instruction::markEnd() {
    for (unsigned int i = 1; i < LENGTH + 1; i++) {
        if (buf[i] == '\0') {
            buf[i + 1] = ETB;
            return;
        }
    }
    buf[LENGTH + 1] = ETB;
}

bool Instruction::fromBuf(const char* buf) {
    mosi = true;
    code = buf[0];
    memcpy(data, buf + 1, LENGTH);
    bool valid = false;
    size_t size = codeSize(code, true);
    if (size != LENGTH_DYNAMIC) {
        valid = buf[size + 1] == ETB;
    } else if (code == MOSI_REQUEST) {
        valid = indexOf(buf + 1, ETB, LENGTH + 1) != NOT_FOUND;
    } else if (code == MOSI_TEXT) {
        size_t i = indexOf(buf + 1, '\0', LENGTH);
        valid = data[i == NOT_FOUND ? LENGTH : i + 1] == ETB;
    }
    if (valid) {
        return true;
    } else {
        clear();
        return false;
    }
}

void Instruction::clear() {
    code = INST_ERROR;
    memset(data, 0, LENGTH);
}

bool Instruction::isValid() const {
    return code != INST_ERROR;
}

bool Instruction::isFinished() const {
    return (mosi && code != MOSI_TEXT) || (!mosi && code != MISO_TEXT)
           || indexOf(data+1, '\0', LENGTH) != NOT_FOUND;
}

char* Instruction::getData() const {
    memcpy(buf, data, LENGTH + 1);
    if (((mosi && code == MOSI_TEXT) || (!mosi && code == MISO_TEXT))
        && buf[LENGTH] != ETB) {
        buf[LENGTH] = '\0';
        buf[LENGTH] = ETB;
    }
    return buf;
}

char Instruction::getData(size_t index) const {
    return data[index + 1];
}

char Instruction::getCode() const {
    return code;
}

class Bluetooth {
public:
  Bluetooth(int ss, int irq, int rst);
  void init();
  bool available();
  void send(const Instruction &ins);
  void read(Instruction &ins);

private:
  static char buffer[25];
  Adafruit_BluefruitLE_SPI ble;
};

char Bluetooth::buffer[25];

Bluetooth::Bluetooth(int ss, int irq, int rst) : ble(ss, irq, rst) {}

void Bluetooth::init() {
  ble.begin(false);
  ble.setMode(BLUEFRUIT_MODE_DATA);
}

bool Bluetooth::available() {
  return ble.available();
}

void Bluetooth::send(const Instruction &ins) {
  char* buf = ins.toBuf();
  ble.write(ins.toBuf(), indexOf(buf, Instruction::ETB, 20));
}

void Bluetooth::read(Instruction &ins) {
  char c;
  for(size_t i = 0; (i < 25) && (ble.available())
      && (c != Instruction::ETB); i++) {
    c = ble.read();
    buffer[i] = c;
  }
  ins.fromBuf(buffer);
}

#endif
