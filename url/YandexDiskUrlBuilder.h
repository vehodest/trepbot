#pragma once

#include "UrlBuilder.h"

// https://tech.yandex.ru/disk/api/concepts/about-docpage/

class YandexDiskUrlBuilder final : public UrlBuilder {
 public:
  YandexDiskUrlBuilder(Escaper& escaper);
  std::string Info();
  std::string Upload(std::string const& path, bool overwrite);
  std::string Delete(std::string const& path, bool permanently);

 private:
  static std::string const baseUrl;
};