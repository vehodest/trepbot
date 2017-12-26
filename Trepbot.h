#pragma once

#include "Escaper.h"
#include "Httper.h"
#include "TelegramUrlBuilder.h"
#include "YandexUrlBuilder.h"
#include "json.hpp"

#include <map>

class Trepbot final {
 public:
  Trepbot(std::string const& telegramToken,
          std::string const& yandexToken,
          Httper& http,
          Escaper& esc);

  Trepbot(Trepbot const&) = delete;
  Trepbot& operator=(Trepbot const&) = delete;

  void ProcessUpdates();
  void GetInfo();

 private:
  using Handler = void (Trepbot::*)(nlohmann::json&);
  using handlersType = std::map<std::string, Handler>;
  static handlersType handlers;

  Httper& http;
  Escaper& esc;

  TelegramUrlBuilder tUrl;
  YandexUrlBuilder yaUrl;

  size_t offset;

  size_t ownId;
  std::string name;

  void MessageHandler(nlohmann::json& msg);
  void QueryHandler(nlohmann::json& query);
};
