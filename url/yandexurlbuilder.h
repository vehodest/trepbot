#pragma once
#include <urlbuilder.h>

#include <map>
#include <sstream>
#include <string>

// https://tech.yandex.ru/speechkit/cloud/doc/guide/concepts/tts-http-request-docpage/

class YandexUrlBuilder final : public UrlBuilder {
 public:
  YandexUrlBuilder(std::string const& apiKey, Escaper& escaper);

  YandexUrlBuilder(YandexUrlBuilder const&) = delete;
  YandexUrlBuilder& operator=(YandexUrlBuilder const&) = delete;

  enum Format { mp3, wav, opus };

  enum Quality { high, low };

  enum Language { ru, en, uk, tr };

  enum Speaker { Jane, Oksana, Alyss, Omazh, Zahar, Ermil };

  enum Emotion { good, neutral, evil };

  std::string Make(std::string const& text,
                   Speaker speaker = Speaker::Jane,
                   Emotion emotion = Emotion::good) const;

 private:
  static const std::map<Format, std::string> formats;
  static const std::map<Quality, std::string> qualities;
  static const std::map<Language, std::string> languages;
  static const std::map<Speaker, std::string> speakers;
  static const std::map<Emotion, std::string> emotions;

  Format format;
  Quality quality;
  Language language;
  float speed;
};