#pragma once
#include <escaper.h>

#include <string>

class UrlBuilder {
 public:
  UrlBuilder(std::string const& token, Escaper& escaper)
      : token(token), escaper(escaper) {}
  UrlBuilder(UrlBuilder const&) = delete;

 protected:
  std::string const token;
  Escaper& escaper;
};