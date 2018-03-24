cmake_minimum_required (VERSION 3.5)

project (trepbot)

set(TELEGRAM "" CACHE STRING "Telegram bot token")
set(YANDEX "" CACHE STRING "Yandex speech token")

add_definitions("-std=c++11 -Wall")
add_definitions("-DTELEGRAM_TOKEN=\"${TELEGRAM}\"")
add_definitions("-DYANDEX_TOKEN=\"${YANDEX}\"")

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/url/TelegramUrlBuilder.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/url/YandexUrlBuilder.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/transport/CurlEngine.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Basebot.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Trepbot.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)


include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/transport")
include_directories ("${PROJECT_SOURCE_DIR}/url")
include_directories ("${PROJECT_SOURCE_DIR}/3rd/json/src")

add_executable(trepbot ${SOURCE})

target_link_libraries(trepbot curl)
