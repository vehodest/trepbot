#pragma once

#include <sstream>
#include <string>
#include "UrlBuilder.h"

class TelegramUrlBuilder final : public UrlBuilder {
 public:
  TelegramUrlBuilder(std::string const& token, Escaper& escaper);
  TelegramUrlBuilder(TelegramUrlBuilder const&) = delete;
  TelegramUrlBuilder& operator=(TelegramUrlBuilder const&) = delete;

  std::string SendVoice(long int chatId) const;
  std::string SendMessage(long int chatId, std::string const& message) const;
  std::string GetUpdates(size_t timeout, size_t offset) const;
  std::string AnswerInlineQuery(std::string queryId) const;

 private:
  static std::string const telegramMainUrl;

  std::string const telegramUrl;
  std::stringstream PrepareUrl(std::string const& methodName) const;
};