CC = g++
CFLAGS := --std=c++11
CFLAGS += -DTELEGRAM_TOKEN=\"$(TELEGRAM)\"
CFLAGS += -DYANDEX_TOKEN=\"$(YANDEX)\"

LIBS = -lcurl

INCLUDES := -Itransport
INCLUDES += -Iurl
INCLUDES += -I3rd/json/src
INCLUDES += -I./

FILES := url/TelegramUrlBuilder.cpp
FILES += url/YandexUrlBuilder.cpp
FILES += transport/CurlEngine.cpp
FILES += Basebot.cpp
FILES += Trepbot.cpp
FILES += main.cpp

all:
	$(CC) $(CFLAGS) $(INCLUDES) -o trepbot $(FILES) $(LIBS)

clean:
	rm -f trepbot
