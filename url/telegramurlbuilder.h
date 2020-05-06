#pragma once
#include <urlbuilder.h>

#include <sstream>
#include <string>

class TelegramUrlBuilder final : public UrlBuilder {
 public:
  TelegramUrlBuilder(std::string const& token, Escaper& escaper);
  TelegramUrlBuilder(TelegramUrlBuilder const&) = delete;
  TelegramUrlBuilder& operator=(TelegramUrlBuilder const&) = delete;

  std::string SendVoice(long int chatId, bool sendNotify) const;
  std::string SendMessage(long int chatId, std::string const& message) const;
  std::string GetUpdates(size_t timeout, size_t offset) const;
  std::string AnswerInlineQuery(std::string const& queryId,
                                std::string const& answer) const;
  std::string GetMe() const;
  std::string DeleteMessage(size_t chatId, size_t messageId) const;

 private:
  static std::string const telegramMainUrl;

  std::string const telegramUrl;
  std::stringstream PrepareUrl(std::string const& methodName) const;
};