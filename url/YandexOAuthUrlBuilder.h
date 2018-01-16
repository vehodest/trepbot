#pragma once

#include "UrlBuilder.h"

// https://tech.yandex.ru/oauth/doc/dg/reference/web-client-docpage/

class YandexOAuthUrlBuilder final : public UrlBuilder {
 public:
  YandexOAuthUrlBuilder(std::string const& appId, Escaper& escaper);

  std::string Info();
  std::string Authorize();

 private:
  static std::string const baseUrl;
};