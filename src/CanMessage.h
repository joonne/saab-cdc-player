#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

#include <Arduino.h>

class CanMessageSuccess {
public:
  CanMessageSuccess(unsigned long id, uint8_t data[8]) : id(id) {
    for (int i = 0; i < 8; i++) {
      this->data[i] = data[i];
    }
  }
  unsigned long id;
  uint8_t data[8];
};

class CanMessageFailure {
public:
  CanMessageFailure(unsigned long id) : id(id) {}
  unsigned long id;
};

union CanMessage {
  CanMessage(unsigned long id, uint8_t data[8]) : success(id, data) {}
  CanMessage(unsigned long id) : failure(id) {}
  CanMessageSuccess success;
  CanMessageFailure failure;
};

#endif // CAN_MESSAGE_H
