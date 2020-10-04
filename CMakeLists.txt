cmake_minimum_required(VERSION 3.0)

project(utcoupe_nextgen_asserv)

add_executable(utcoupe_nextgen_asserv src/main.cpp)

install(TARGETS utcoupe_nextgen_asserv RUNTIME DESTINATION bin)
