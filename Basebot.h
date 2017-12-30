#pragma once

#include "Escaper.h"
#include "Httper.h"
#include "TelegramUrlBuilder.h"
#include "YandexUrlBuilder.h"
#include "json.hpp"

#include <map>

class Basebot {
 public:
  Basebot(std::string const& telegramToken,
          Httper& http,
          Escaper& esc);

  Basebot(Basebot const&) = delete;
  Basebot& operator=(Basebot const&) = delete;

  void ProcessUpdates();
  void GetInfo(size_t ownId, std::string& name);

 protected:
  using Handler = void (Basebot::*)(nlohmann::json&);
  using handlersType = std::map<std::string, Handler>;
  handlersType handlers;

  Httper& http;
  Escaper& esc;

  TelegramUrlBuilder tUrl;

 private:
  size_t offset;
};
