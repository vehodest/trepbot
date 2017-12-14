#pragma once

#include <sstream>
#include <string>
#include "UrlBuilder.h"

// https://tech.yandex.ru/speechkit/cloud/doc/guide/concepts/tts-http-request-docpage/

/* https://tts.voicetech.yandex.net/generate?
   key=<API‑ключ>
   & text=<текст>
   & format=<mp3|wav|opus>
   & [quality=<hi|lo>]
   & lang=<ru-RU|en-US|uk-UK|tr-TR>
   & speaker=<jane|oksana|alyss|omazh|zahar|ermil>
   & [speed=<скорость речи>]
   & [emotion=<good|neutral|evil>]
//*/

class YandexUrlBuilder final : public UrlBuilder {
 public:
  YandexUrlBuilder(std::string const& apiKey, Escaper& escaper)
      : UrlBuilder(apiKey, escaper) {}
  YandexUrlBuilder(YandexUrlBuilder const&) = delete;
  YandexUrlBuilder& operator=(YandexUrlBuilder const&) = delete;

  std::string Make(std::string const& text) const {
    std::stringstream s;
    s << "https://tts.voicetech.yandex.net/generate?"
      << "key=" << token << "&text=" << escaper.Escape(text) << "&format=opus"
      << "&quality=hi"
      << "&lang=ru-RU"
      << "&speaker=jane"
      << "&speed=1.0"
      << "&emotion=good";

    return s.str();
  }
};