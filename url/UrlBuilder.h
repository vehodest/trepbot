#pragma once

#include <string>
#include "Escaper.h"

class UrlBuilder {
 public:
  UrlBuilder(std::string const& token, Escaper& escaper)
      : token(token), escaper(escaper) {}
  UrlBuilder(UrlBuilder const&) = delete;

 protected:
  std::string const token;
  Escaper& escaper;
};