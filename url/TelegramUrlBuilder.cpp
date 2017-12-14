#include "TelegramUrlBuilder.h"

std::string const TelegramUrlBuilder::telegramMainUrl(
    "https://api.telegram.org/bot");

TelegramUrlBuilder::TelegramUrlBuilder(std::string const& token,
                                       Escaper& escaper)
    : UrlBuilder(token, escaper), telegramUrl(telegramMainUrl + token + "/") {}

std::string TelegramUrlBuilder::SendVoice(long int chatId) const {
  std::stringstream str(PrepareUrl("sendVoice"));

  str << "?chat_id=" << chatId;

  return str.str();
}

std::string TelegramUrlBuilder::SendMessage(long int chatId,
                                            std::string const& message) const {
  std::stringstream str(PrepareUrl("sendMessage"));

  str << "?chat_id=" << chatId << "&text=" << escaper.Escape(message);

  return str.str();
}

std::string TelegramUrlBuilder::GetUpdates(size_t timeout,
                                           size_t offset) const {
  std::stringstream str(PrepareUrl("getUpdates"));

  str << "?timeout=" << timeout << "&offset=" << offset;

  return str.str();
}

std::string TelegramUrlBuilder::AnswerInlineQuery(std::string queryId) const {
  std::stringstream str(PrepareUrl("answerInlineQuery"));

  str << "?inline_query_id=" << queryId << "&results=[]";

  return str.str();
}

std::stringstream TelegramUrlBuilder::PrepareUrl(
    std::string const& methodName) const {
  std::stringstream str;
  str << telegramUrl << methodName;
  return str;
}