cmake_minimum_required (VERSION 3.5)

project (trepbot)

set(TELEGRAM "" CACHE STRING "Telegram bot token")
set(YANDEX "" CACHE STRING "Yandex speech token")

add_definitions("-std=c++11 -Wall")
add_definitions("-DTELEGRAM_TOKEN=\"${TELEGRAM}\"")
add_definitions("-DYANDEX_TOKEN=\"${YANDEX}\"")

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/url/telegramurlbuilder.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/url/yandexurlbuilder.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/transport/curlengine.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/basebot.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/trepbot.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)


include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/transport")
include_directories ("${PROJECT_SOURCE_DIR}/url")
include_directories ("${PROJECT_SOURCE_DIR}/thirdparty")

add_executable(trepbot ${SOURCE})

target_link_libraries(trepbot curl)
