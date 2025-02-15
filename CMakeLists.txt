cmake_minimum_required(VERSION 3.16)
project(music_visualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

include_directories(include)

set(OPENSSL_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openssl_example.c
  )

add_executable(openssl_example)
target_sources(openssl_example
 PRIVATE
  ${OPENSSL_SRC}
)

target_link_libraries(openssl_example OpenSSL::SSL)
