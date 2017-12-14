#pragma once

#include "StopFlag.h"

class SimpleStopFlag final : public StopFlag {
 public:
  SimpleStopFlag() : value(false) {}
  SimpleStopFlag(SimpleStopFlag const&) = delete;
  SimpleStopFlag& operator=(SimpleStopFlag const&) = delete;

  bool IsStop() const override { return value; }

  void Stop() override { value = true; }

 private:
  volatile bool value;
};