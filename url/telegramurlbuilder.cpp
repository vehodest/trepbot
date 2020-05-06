#include <telegramurlbuilder.h>

std::string const TelegramUrlBuilder::telegramMainUrl(
    "https://api.telegram.org/bot");

TelegramUrlBuilder::TelegramUrlBuilder(std::string const& token,
                                       Escaper& escaper)
    : UrlBuilder(token, escaper), telegramUrl(telegramMainUrl + token + "/") {}

std::string TelegramUrlBuilder::SendVoice(long int chatId,
                                          bool sendNotify) const {
  std::stringstream str(PrepareUrl("sendVoice"));

  str << "?chat_id=" << chatId
      << "&disable_notification=" << (sendNotify ? "true" : "false");

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

std::string TelegramUrlBuilder::AnswerInlineQuery(
    std::string const& queryId,
    std::string const& answer) const {
  std::stringstream str(PrepareUrl("answerInlineQuery"));

  str << "?inline_query_id=" << queryId << "&results=" << answer;

  return str.str();
}

std::string TelegramUrlBuilder::GetMe() const {
  std::stringstream str(PrepareUrl("getMe"));

  return str.str();
}

std::string TelegramUrlBuilder::DeleteMessage(size_t chatId,
                                              size_t messageId) const {
  std::stringstream str(PrepareUrl("deleteMessage"));

  str << "?chat_id=" << chatId << "&message_id=" << messageId;

  return str.str();
}

std::stringstream TelegramUrlBuilder::PrepareUrl(
    std::string const& methodName) const {
  std::stringstream str;
  str << telegramUrl << methodName;
  return str;
}