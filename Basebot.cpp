#include "Basebot.h"
#include "Escaper.h"
#include "Httper.h"
#include "StopFlag.h"

#include <iostream>

Basebot::Basebot(std::string const& telegramToken, Httper& http, Escaper& esc)
    : offset(0), http(http), esc(esc), tUrl(telegramToken, esc) {}

void Basebot::Run(StopFlag& stopFlag) {
  while (!stopFlag.IsStop()) {
    try {
      ProcessUpdates();
    } catch (...) {
      std::cout << "Unknown exception" << std::endl;
      break;
    }
  }
}

void Basebot::GetInfo(size_t ownId, std::string& name) {
  Httper::ContainerType data = http.Get(tUrl.GetMe());

  data.push_back(char(0));
  nlohmann::json answer = nlohmann::json::parse(data.data());

  ownId = answer["result"]["id"].get<size_t>();
  name = answer["result"]["username"].get<std::string>();
}

void Basebot::ProcessUpdates() {
  // Получаем данные из Телеграма
  http.SetTimeout(60500);
  std::cout << tUrl.GetUpdates(60, offset) << std::endl;
  Httper::ContainerType data = http.Get(tUrl.GetUpdates(60, offset));

  data.push_back(char(0));
  nlohmann::json answer = nlohmann::json::parse(data.data());

  std::cout << "Message: " << std::endl << data.data() << std::endl;

  // Обрабатывем их
  if (false == answer["ok"].get<bool>()) {
    std::cout << "Error" << std::endl;
    return;
  }

  nlohmann::json result = answer["result"];
  size_t max(0);
  for (auto& update : result) {  // перебор элементов секции Update
    for (auto obj = update.begin(); obj != update.end(); ++obj) {
      if (obj.key() == "update_id") {  // определение максимального элемента
        long int id = obj->get<long int>();
        if (id > max) {
          max = id;
        }
      } else if (obj->is_object()) {  // получение имени объекта
        auto it = handlers.find(obj.key());

        if (handlers.end() != it) {
          (this->*(it->second))(*obj);  // вызов хендлера
        }
      }
    }
  }

  offset = max + 1;
}
