#include "CanMessage.h"
#include "CanTask.h"
#include "CdChanger.h"
#include "SteeringWheelButtons.h"
#include "Task.h"
#include "communication.h"
#include "defines.h"
#include "mcp_can.h"
#include "mcp_can_dfs.h"
#include <Arduino.h>

MCP_CAN CAN(CAN_CS_PIN);

CanMessage readCanBus(MCP_CAN can) {
  uint8_t len;
  uint8_t data[8];
  unsigned long id;

  if (CAN.checkReceive() == CAN_NOMSG) {
    CanMessage empty = {
        .status = CAN_MESSAGE_STATUS::NO_MESSAGE,
        .value = {.failure = CanMessageFailure()},
    };

    return empty;
  }

  if (can.readMsgBuf(&id, &len, data) == CAN_OK) {
    CanMessage success = {
        .status = CAN_MESSAGE_STATUS::SUCCESS,
        .value = {.success = CanMessageSuccess(id, data)},
    };

    return success;
  }

  CanMessage failure = {
      .status = CAN_MESSAGE_STATUS::FAILURE,
      .value = {.failure = CanMessageFailure()},
  };

  return failure;
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
              return CdChanger::handshake(CAN);
            }),
    CanTask(CAN_MESSAGE_ID_IN::STEERING_WHEEL_BUTTONS,
            [](CanMessage message) {
              Serial.println("STEEERING_WHEEL_BUTTONS");
              return SteeringWheelButtons::onFrame(message.value.success.data);
            }),
};

Task loopTasks[2] = {
    Task([]() { CdChanger::heartbeat(CAN); }),
    Task([]() {
      auto message = readCanBus(CAN);

      if (message.isEmpty() || message.isFailure()) {
        return;
      }

      for (auto &task : canTasks) {
        if (task.id ==
            static_cast<CAN_MESSAGE_ID_IN>(message.value.success.id)) {
          task.run(message);
        }
      }
    }),
};

// we have tasks that
// * run on setup
// * run on loop
// * run on every can message

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
