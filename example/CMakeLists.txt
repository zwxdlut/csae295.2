cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_compile_options(-Wall -O0 -g -rdynamic)
add_definitions(-D _UDEBUG)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/protocol
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/protocol/data
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/protocol/jt808
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/protocol/jt1078
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/protocol/message
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/util
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../src SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../src/protocol SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../src/util SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} pthread rt)
