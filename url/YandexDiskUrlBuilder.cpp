#include "YandexDiskUrlBuilder.h"

std::string const YandexDiskUrlBuilder::baseUrl(
    "https://cloud-api.yandex.net/v1/disk/");

YandexDiskUrlBuilder::YandexDiskUrlBuilder(Escaper& escaper)
    : UrlBuilder(std::string(), escaper) {}

std::string YandexDiskUrlBuilder::Info() {
  return baseUrl;
}

std::string YandexDiskUrlBuilder::Upload(std::string const& path,
                                         bool overwrite) {
  return baseUrl + "resources/upload?path=" + escaper.Escape(path) +
         "&overwrite=" + (overwrite ? "true" : "false");
}

std::string YandexDiskUrlBuilder::Delete(std::string const& path,
                                         bool permanently) {
  return baseUrl + "resources?path=" + escaper.Escape(path) +
         "&permanently=" + (permanently ? "true" : "false");
}

std::string YandexDiskUrlBuilder::Publish(std::string const& path) {
  return baseUrl + "resources/publish?path=" + escaper.Escape(path);
}

std::string YandexDiskUrlBuilder::Unpublish(std::string const& path) {
  return baseUrl + "resources/unpublish?path=" + escaper.Escape(path);
}

std::string YandexDiskUrlBuilder::Download(std::string const& publicKey) {
  return baseUrl +
         "public/resources/download?public_key=" + escaper.Escape(publicKey);
}