#pragma once

#include "UrlBuilder.h"

// https://tech.yandex.ru/disk/api/concepts/about-docpage/

class YandexDiskUrlBuilder final : public UrlBuilder {
 public:
  YandexDiskUrlBuilder(Escaper& escaper);
  std::string Info();
  std::string Upload(std::string const& path, bool overwrite);
  std::string Delete(std::string const& path, bool permanently);
  std::string Publish(std::string const& path);
  std::string Unpublish(std::string const& path);
  std::string Download(std::string const& publicKey);

 private:
  static std::string const baseUrl;
};