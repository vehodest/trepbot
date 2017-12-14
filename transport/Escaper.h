#pragma once

#include <string>

class Escaper {
 public:
  virtual std::string Escape(std::string const& str) = 0;
};