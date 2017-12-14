#include <csignal>
#include <iostream>

#include "CurlEngine.h"
#include "SimpleStopFlag.h"
#include "Trepbot.h"

#ifndef YANDEX_TOKEN
#define YANDEX_TOKEN ""
#endif

#ifndef TELEGRAM_TOKEN
#define TELEGRAM_TOKEN ""
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
  Trepbot bot(TELEGRAM_TOKEN, YANDEX_TOKEN, curlEng, curlEng);

  while (true) {
    try {
      bot.ProcessUpdates();
    } catch (...) {
      if (stopFlag.IsStop()) {
        break;
      }
    }
  }

  std::cout << "That's all!" << std::endl;
  return 0;
}
