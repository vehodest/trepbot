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

 private:
  typedef void (Trepbot::*Handler)(nlohmann::json&);
  typedef std::map<std::string, Handler> handlersType;
  static handlersType handlers;

  Httper& http;
  Escaper& esc;

  TelegramUrlBuilder tUrl;
  YandexUrlBuilder yaUrl;

  size_t offset;

  void MessageHandler(nlohmann::json& msg);
  void QueryHandler(nlohmann::json& query);
};