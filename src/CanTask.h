#ifndef CANTASK_H
#define CANTASK_H

#include "CanMessage.h"
#include "communication.h"

class CanTask {
public:
  CanTask(CAN_MESSAGE_ID_IN id, void (*aFn)(CanMessage message))
      : id(id), fn(aFn) {}
  CAN_MESSAGE_ID_IN id;
  void run(CanMessage message) { fn(message); }

private:
  void (*fn)(CanMessage message);
};

#endif // CANTASK_H
