#ifndef HELLOROBOTICS_UTILS_H
#define HELLOROBOTICS_UTILS_H

#include <Arduino.h>

const size_t NOT_FOUND = -1;

void printDbg(const char* text) {
  Serial.println("DBG:");
  Serial.println(text);
}

void printDbg() {
  Serial.println("DBG!");
}

bool getUserInput(char buffer[], uint8_t maxSize) {
  if(!Serial.available()) return false;
  memset(buffer, 0, maxSize);
  Serial.readBytes(buffer, maxSize);
  return true;
}

size_t indexOf(const char* data, char toFind, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (*data == toFind) {
      return i;
    } else {
      data++;
    }
  }
  return NOT_FOUND;
}

#endif
