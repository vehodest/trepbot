#include "YandexOAuthUrlBuilder.h"

std::string const YandexOAuthUrlBuilder::baseUrl("https://oauth.yandex.ru/");

YandexOAuthUrlBuilder::YandexOAuthUrlBuilder(std::string const& appId,
                                             Escaper& escaper)
    : UrlBuilder(appId, escaper) {}

std::string YandexOAuthUrlBuilder::Info() {
  return baseUrl + "client/" + token + "/info";
}

std::string YandexOAuthUrlBuilder::Authorize() {
  return baseUrl + "authorize?response_type=token&client_id=" + token;
}