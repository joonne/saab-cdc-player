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
  CanMessageFailure() {}
};

struct CanMessage {
  const char *type;
  union {
    CanMessageSuccess success;
    CanMessageFailure failure;
  } value;
};

#endif // CAN_MESSAGE_H
