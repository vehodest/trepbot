#pragma once

class StopFlag {
 public:
  virtual bool IsStop() const = 0;
  virtual void Stop() = 0;
};