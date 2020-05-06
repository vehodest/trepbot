#include <yandexurlbuilder.h>

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

const std::map<YandexUrlBuilder::Format, std::string>
    YandexUrlBuilder::formats = {{YandexUrlBuilder::Format::mp3, "mp3"},
                                 {YandexUrlBuilder::Format::wav, "wav"},
                                 {YandexUrlBuilder::Format::opus, "opus"}};

const std::map<YandexUrlBuilder::Quality, std::string>
    YandexUrlBuilder::qualities = {{YandexUrlBuilder::Quality::high, "hi"},
                                   {YandexUrlBuilder::Quality::low, "lo"}};

const std::map<YandexUrlBuilder::Language, std::string>
    YandexUrlBuilder::languages = {
        {YandexUrlBuilder::Language::ru, "ru-RU"},
        {YandexUrlBuilder::Language::en, "en-US"},
        {YandexUrlBuilder::Language::uk, "uk-UK"},
        {YandexUrlBuilder::Language::tr, "tr-TR"},
};

const std::map<YandexUrlBuilder::Speaker, std::string>
    YandexUrlBuilder::speakers = {{YandexUrlBuilder::Speaker::Jane, "jane"},
                                  {YandexUrlBuilder::Speaker::Oksana, "oksana"},
                                  {YandexUrlBuilder::Speaker::Alyss, "alyss"},
                                  {YandexUrlBuilder::Speaker::Omazh, "omazh"},
                                  {YandexUrlBuilder::Speaker::Zahar, "zahar"},
                                  {YandexUrlBuilder::Speaker::Ermil, "ermil"}};

const std::map<YandexUrlBuilder::Emotion, std::string>
    YandexUrlBuilder::emotions = {
        {YandexUrlBuilder::Emotion::good, "good"},
        {YandexUrlBuilder::Emotion::neutral, "neutral"},
        {YandexUrlBuilder::Emotion::evil, "evil"}};

YandexUrlBuilder::YandexUrlBuilder(std::string const& apiKey, Escaper& escaper)
    : UrlBuilder(apiKey, escaper),
      format(Format::mp3),
      quality(Quality::high),
      language(Language::ru),
      speed(1.0) {}

std::string YandexUrlBuilder::Make(std::string const& text,
                                   Speaker speaker,
                                   Emotion emotion) const {
  std::stringstream s;
  s << "https://tts.voicetech.yandex.net/generate?"
    << "key=" << token << "&text=" << escaper.Escape(text)
    << "&format=" << formats.at(format) << "&quality=" << qualities.at(quality)
    << "&lang=" << languages.at(language) << "&speaker=" << speakers.at(speaker)
    << "&speed=" << speed << "&emotion=" << emotions.at(emotion);

  return s.str();
}
