CC = g++
CFLAGS := --std=c++11
CFLAGS += -DTELEGRAM_TOKEN=\"$(TELEGRAM)\"
CFLAGS += -DYANDEX_SPEACH_TOKEN=\"$(YANDEX)\"
CFLAGS += -DYANDEX_DISK_ID=\"$(ID)\"
CFLAGS += -DYANDEX_DISK_TOKEN=\"$(DISK)\"

LIBS = -lcurl

INCLUDES := -Itransport
INCLUDES += -Iurl
INCLUDES += -I3rd/json/src
INCLUDES += -I./

FILES := url/TelegramUrlBuilder.cpp
FILES += url/YandexUrlBuilder.cpp
FILES += url/YandexOAuthUrlBuilder.cpp
FILES += url/YandexDiskUrlBuilder.cpp
FILES += transport/CurlEngine.cpp
FILES += Basebot.cpp
FILES += Trepbot.cpp
FILES += main.cpp

all:
	$(CC) $(CFLAGS) $(INCLUDES) -o trepbot $(FILES) $(LIBS)

clean:
	rm -f trepbot
