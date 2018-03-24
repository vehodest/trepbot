# trepbot
__Make Telegram bot using C++ for FUN!__

First usage:
```bash
sudo apt update
sudo apt install libcurl4-openssl-dev

git clone git@github.com:vehodest/trepbot.git
git submodule init
git submodule update

mkdir build; cd build
cmake .. -DTELEGRAM=<your Telegram bot token> -DYANDEX=<your Yandex speech token>
make
```
