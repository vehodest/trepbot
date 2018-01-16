#include <csignal>
#include <iostream>

#include "CurlEngine.h"
#include "SimpleStopFlag.h"
#include "Trepbot.h"
#include "YandexDiskUrlBuilder.h"
#include "YandexOAuthUrlBuilder.h"

#ifndef TELEGRAM_TOKEN
#define TELEGRAM_TOKEN ""
#endif

#ifndef YANDEX_SPEACH_TOKEN
#define YANDEX_SPEACH_TOKEN ""
#endif

#ifndef DYANDEX_DISK_ID
#define DYANDEX_DISK_ID ""
#endif

#ifndef YANDEX_DISK_TOKEN
#define YANDEX_DISK_TOKEN ""
#endif

namespace {
SimpleStopFlag stopFlag;

void signal_handler(int signum) {
  stopFlag.Stop();
}
}  // namespace

int main() {
  std::signal(SIGTERM, signal_handler);
  std::signal(SIGINT, signal_handler);

  CurlEngine curlEng(stopFlag);
  curlEng.AddHeader("Authorization: OAuth " YANDEX_DISK_TOKEN);
  // Trepbot bot(TELEGRAM_TOKEN, YANDEX_SPEACH_TOKEN, curlEng, curlEng);
  // bot.Run(stopFlag);

  // YandexOAuthUrlBuilder test(DYANDEX_DISK_ID, curlEng);
  YandexDiskUrlBuilder test(curlEng);

  auto res = curlEng.Get(test.Info());
  res.push_back(0);
  nlohmann::json inf = nlohmann::json::parse(res.data());
  std::cout << inf.dump(2) << std::endl;

  res = curlEng.Get(test.Upload("app:/test.txt", true));
  res.push_back(0);

  nlohmann::json result = nlohmann::json::parse(res.data());
  std::cout << result.dump(2) << std::endl;

  if (!result["href"].empty()) {
    CurlEngine::ContainerType file = {'1', '2', '3', '4', ' ', 'H', 'i', '!'};
    std::cout << "PUT result code: "
              << curlEng.Put(result["href"].get<std::string>(), file)
              << std::endl;

    std::cout << "DELETE result code: "
              << curlEng.Delete(test.Delete("app:/test.txt", true))
              << std::endl;
  }

  std::cout << "That's all!" << std::endl;
  return 0;
}
