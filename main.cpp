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

  Httper::ContainerType res;
  curlEng.Get(test.Info(), res);
  res.push_back(0);

  nlohmann::json inf = nlohmann::json::parse(res.data());
  std::cout << inf.dump(2) << std::endl;

  std::string const path("app:/test.txt");
  curlEng.Get(test.Upload(path, true), res);
  res.push_back(0);

  nlohmann::json result = nlohmann::json::parse(res.data());
  std::cout << result.dump(2) << std::endl;

  if (!result["href"].empty()) {
    CurlEngine::ContainerType file = {'#', 'p', 'n', 'z', 't', 'r', 'i', 'p'};
    std::cout << "PUT result code: "
              << curlEng.Put(result["href"].get<std::string>(), file, res)
              << std::endl;

    if (200 ==
        curlEng.Put(test.Publish(path), CurlEngine::ContainerType(), res)) {
      res.push_back(0);
      result = nlohmann::json::parse(res.data());
      if (!result["href"].empty()) {
        std::string href = result["href"].get<std::string>();
        curlEng.Get(href, res);
        res.push_back(0);
        result = nlohmann::json::parse(res.data());
        if (!result["public_url"].empty()) {
          std::cout << "PUBLISH URL: "
                    << result["public_url"].get<std::string>() << std::endl;
        }

        if (!result["public_key"].empty()) {
          curlEng.Get(test.Download(result["public_key"].get<std::string>()),
                      res);
          res.push_back(0);
          result = nlohmann::json::parse(res.data());
          std::string href = result["href"].get<std::string>();
          std::cout << "DOWNLOAD URL: " << href << std::endl;
        }
      }
    }

    /*
    std::cout << "DELETE result code: "
              << curlEng.Delete(test.Delete(path, true), res)
              << std::endl; //*/
  }

  std::cout << "That's all!" << std::endl;
  return 0;
}
