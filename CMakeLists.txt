cmake_minimum_required(VERSION 3.10)
project(auction)

set(CMAKE_BUILD_TYPE Debug)

include_directories(./)
include_directories(libs/botapi/header)
include_directories(libs/data)

include_directories(libs/openssl/include/)
include_directories(libs/openssl/include/openssl)

include_directories(libs/json-develop/include/nlohmann)
include_directories(libs/json-develop/include/)

include_directories(libs/json-develop/include/nlohmann)
include_directories(libs/json-develop/include)

link_directories(libs/openssl)
link_directories(libs/botapi)
link_directories(libs/data)


subdirs(libs/botapi)
subdirs(libs/data)

set(SRC main.cpp func.h test.h)
set(LIBS data botapi ssl crypto dl)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${LIBS})
