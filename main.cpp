#include <curlengine.h>
#include <simplestopflag.h>
#include <trepbot.h>

#include <csignal>
#include <iostream>

#ifndef YANDEX_TOKEN
#define YANDEX_TOKEN ""
#endif

#ifndef TELEGRAM_TOKEN
#define TELEGRAM_TOKEN ""
#endif

namespace {
  
SimpleStopFlag stopFlag;

void signal_handler(int signum)
{
  stopFlag.Stop();
}

}  // unnamed namespace

int main() {
  std::signal(SIGTERM, signal_handler);
  std::signal(SIGINT, signal_handler);

  CurlEngine curlEng(stopFlag);
  Trepbot bot(TELEGRAM_TOKEN, YANDEX_TOKEN, curlEng, curlEng);
  bot.Run(stopFlag);

  std::cout << "That's all!" << std::endl;
  return 0;
}
