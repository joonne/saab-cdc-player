#ifndef TASK_H
#define TASK_H

class Task {
public:
  Task(void (*aFn)()) { fn = aFn; }
  void run() { fn(); }

private:
  void (*fn)();
};

#endif // TASK_H
