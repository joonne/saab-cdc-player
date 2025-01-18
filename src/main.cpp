#include "CdChanger.h"
#include "SteeringWheelButtons.h"
#include "communication.h"
#include "defines.h"
#include "mcp_can.h"
#include "mcp_can_dfs.h"
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

class Task {
public:
  Task(void (*aFn)()) { fn = aFn; }
  void run() { fn(); }

private:
  void (*fn)();
};

class CanTask {
public:
  CanTask(CAN_MESSAGE_ID_IN id, void (*aFn)(CanMessage message)) { fn = aFn; }
  CAN_MESSAGE_ID_IN id;
  void run(CanMessage message) { fn(message); }

private:
  void (*fn)(CanMessage message);
};

MCP_CAN CAN(CAN_CS_PIN);
CdChanger CDC(CAN);

CanMessage readCanBus() {
  uint8_t len;
  uint8_t data[8];
  unsigned long id;

  if (CAN.readMsgBuf(&id, &len, data) == CAN_OK) {
    return CanMessage(id, data);
  }

  return CanMessage(0);
}

Task setupTasks[2] = {
    Task([]() { Serial.begin(115200); }),
    Task([]() {
      while (CAN.begin(MCP_ANY, I_BUS, MCP_8MHZ) != CAN_OK) {
        Serial.println("CAN BUS FAIL!");
        delay(100);
      }
      CAN.setMode(MCP_NORMAL);
    }),
};

CanTask canTasks[2] = {
    CanTask(CAN_MESSAGE_ID_IN::CDC_HANDSHAKE_REQUEST,
            [](CanMessage message) {
              Serial.println("CDC_HANDSHAKE_REQUEST");
              return CDC.handshake();
            }),
    CanTask(CAN_MESSAGE_ID_IN::STEERING_WHEEL_BUTTONS,
            [](CanMessage message) {
              Serial.println("STEEERING_WHEEL_BUTTONS");
              return SteeringWheelButtons::onFrame(message.success.data);
            }),
};

Task loopTasks[2] = {
    Task([]() { CDC.heartbeat(); }),
    Task([]() {
      auto message = readCanBus();
      for (auto &task : canTasks) {
        if (task.id == static_cast<CAN_MESSAGE_ID_IN>(message.success.id)) {
          task.run(message);
        }
      }
    }),
};

// we have tasks that
// * run on setup
// * run on loop
// * run on can message

void setup() {
  for (auto &task : setupTasks) {
    task.run();
  }
}

void loop() {
  for (auto &task : loopTasks) {
    task.run();
  }
  delay(500);
}
