#include "Trepbot.h"

#include <iostream>

Trepbot::Trepbot(std::string const& telegramToken,
                 std::string const& yandexToken,
                 Httper& http,
                 Escaper& esc)
    : Basebot(telegramToken, http, esc),
      yaUrl(yandexToken, esc)
{
  handlers["message"] = static_cast<Handler>(&Trepbot::MessageHandler);
  handlers["inline_query"] = static_cast<Handler>(&Trepbot::QueryHandler);
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
  Httper::ContainerType result = std::move(
      http.Post(tUrl.SendVoice(chatId, false), "voice", "voice", data));
  // http.Get(tUrl.SendMessage(chatId, speach));

  result.push_back(0);
  std::cout << "POST result:" << std::endl << result.data() << std::endl;
}

void Trepbot::QueryHandler(nlohmann::json& query) {
  // Получение идентификатора запроса
  std::string queryId = query["id"].get<std::string>();
  size_t senderId = query["from"]["id"].get<size_t>();
  std::string speach = query["query"];

  std::cout << speach << std::endl;

  // Преобразование запроса в речь
  Httper::ContainerType voice = std::move(http.Get(yaUrl.Make(speach)));

  // Отправка запроса себе же на сервера Telegram
  std::cout << "Self query: " << tUrl.SendVoice(senderId, true) << std::endl;
  Httper::ContainerType result = std::move(
      http.Post(tUrl.SendVoice(senderId, true), "voice", "voice", voice));
  result.push_back(char(0));

  nlohmann::json answer = nlohmann::json::parse(result.data());
  if (answer["ok"].get<bool>() != true) {
    return;
  }

  std::string fileId = answer["result"]["voice"]["file_id"];
  size_t messageId = answer["result"]["message_id"].get<size_t>();

  // Формирование и выполнение ответа на запрос
  nlohmann::json cachedVoice;
  cachedVoice["type"] = "voice";
  cachedVoice["id"] = 0;
  cachedVoice["voice_file_id"] = fileId;
  cachedVoice["title"] = speach;

  nlohmann::json queryAnswer;
  queryAnswer = nlohmann::json::array();
  queryAnswer.push_back(cachedVoice);

  Httper::ContainerType data =
      http.Get(tUrl.AnswerInlineQuery(queryId, queryAnswer.dump()));
  data.push_back(0);
  std::cout << "Answer for inline ID: " << queryId << std::endl
            << queryAnswer.dump() << std::endl
            << "Result:" << std::endl
            << data.data() << std::endl;

  // Удаление сообщения
  http.Get(tUrl.DeleteMessage(senderId, messageId));
}