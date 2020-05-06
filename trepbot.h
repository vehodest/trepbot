#pragma once
#include <basebot.h>
#include <yandexurlbuilder.h>

#include <map>

class Trepbot final : public Basebot {
 public:
  Trepbot(std::string const& telegramToken,
          std::string const& yandexToken,
          Httper& http,
          Escaper& esc);

 private:
  YandexUrlBuilder yaUrl;

  void MessageHandler(nlohmann::json& msg);
  void QueryHandler(nlohmann::json& query);
};
