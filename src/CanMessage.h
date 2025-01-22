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

enum class CAN_MESSAGE_STATUS {
  FAILURE,
  SUCCESS,
  NO_MESSAGE,
};

struct CanMessage {
  const CAN_MESSAGE_STATUS status;
  union {
    CanMessageSuccess success;
    CanMessageFailure failure;
  } value;

  bool isFailure() { return status == CAN_MESSAGE_STATUS::FAILURE; }
  bool isSuccess() { return status == CAN_MESSAGE_STATUS::SUCCESS; }
  bool isEmpty() { return status == CAN_MESSAGE_STATUS::NO_MESSAGE; }
};

#endif // CAN_MESSAGE_H
