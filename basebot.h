#pragma once
#include <telegramurlbuilder.h>
#include <yandexurlbuilder.h>
#include <json.hpp>

#include <map>

class StopFlag;
class Httper;
class Escaper;

class Basebot {
 public:
  Basebot(std::string const& telegramToken, Httper& http, Escaper& esc);

  Basebot(Basebot const&) = delete;
  Basebot& operator=(Basebot const&) = delete;

  void Run(StopFlag& stopFlag);
  void GetInfo(size_t ownId, std::string& name);

 protected:
  using Handler = void (Basebot::*)(nlohmann::json&);
  using handlersType = std::map<std::string, Handler>;
  handlersType handlers;

  Httper& http;
  Escaper& esc;

  TelegramUrlBuilder tUrl;

 private:
  void ProcessUpdates();

  size_t offset;
};
