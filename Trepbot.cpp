#include "Trepbot.h"

#include <iostream>

Trepbot::handlersType Trepbot::handlers = {
    {"message", &Trepbot::MessageHandler},
    {"inline_query", &Trepbot::QueryHandler}};

Trepbot::Trepbot(std::string const& telegramToken,
                 std::string const& yandexToken,
                 Httper& http,
                 Escaper& esc)
    : offset(0),
      http(http),
      esc(esc),
      tUrl(telegramToken, esc),
      yaUrl(yandexToken, esc) {}

void Trepbot::ProcessUpdates() {
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

void Trepbot::MessageHandler(nlohmann::json& msg) {
  static const std::string botname("@trepbot");

  if (msg["text"].empty() || msg["chat"].empty()) {
    return;
  }

  std::string speach = msg["text"].get<std::string>();
  size_t chatId = msg["chat"]["id"].get<size_t>();
  std::cout << "Message: " << std::endl
            << "chat: " << chatId << std::endl
            << "text: " << speach << std::endl;

  bool isNeedAnswer(false), isCommand(false);
  for (auto const& entity : msg["entities"]) {
    std::string type = entity["type"].get<std::string>();
    if ("bot_command" == type) {
      isCommand = true;
    } else if ("mention" == type) {
      size_t offset = entity["offset"].get<size_t>();
      size_t length = entity["length"].get<size_t>();

      if (botname == speach.substr(offset, length) && offset == 0) {
        isNeedAnswer = true;
        speach.erase(offset, length);
      }
    }
  }

  if (!isNeedAnswer || isCommand) {
    return;
  }

  // Все данные есть, можно сгенерировать звук и отправить его
  Httper::ContainerType data = std::move(http.Get(yaUrl.Make(speach)));
  Httper::ContainerType result =
      std::move(http.Post(tUrl.SendVoice(chatId), "voice", "voice", data));
  // http.Get(tUrl.SendMessage(chatId, speach));

  result.push_back(0);
  std::cout << "POST result:" << std::endl << result.data() << std::endl;
}

void Trepbot::QueryHandler(nlohmann::json& query) {
  std::cout << "pew" << std::endl;
  std::string id = query["id"].get<std::string>();
  // nlohmann::json result;
  // result["results"] = nlohmann::json::array();

  Httper::ContainerType data = http.Get(tUrl.AnswerInlineQuery(id));
  data.push_back(0);
  std::cout << "Answer for inline ID: " << id << std::endl
            << "Result:" << std::endl
            << data.data() << std::endl;
}