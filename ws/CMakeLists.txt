cmake_minimum_required(VERSION 3.10)

set(OPENSSL_INCLUDE CACHE PATH "Path to the openSSL include directory" )
set(LWS_INCLUDE CACHE PATH "Path to the libwebsodket include directory " )
set(LWS_LIB CACHE PATH "libs" )

if(NOT EXISTS "${OPENSSL_INCLUDE}")
    message(FATAL_ERROR "path of OPENSSL_INCLUDE = ${OPENSSL_INCLUDE} not found!")
endif()

if(NOT EXISTS "${LWS_INCLUDE}")
    message(FATAL_ERROR "path of LWS_INCLUDE = ${LWS_INCLUDE} not found!")
endif()

include_directories(${LWS_INCLUDE})
include_directories(${OPENSSL_INCLUDE})

# Building
project(ws)

set(SOURCES
        include/ws-i.h
        src/Connection.h
        src/Connection.cpp
        src/wsServer.h
        src/wsServer.cpp
        src/wsClient.h
        src/wsClient.cpp
        src/ws.cpp
)

set(LIBS ${LWS_LIB})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

set(LIBS "ws.lib")

link_directories("${CMAKE_CURRENT_BINARY_DIR}")

project(ServerTest)
add_executable(${PROJECT_NAME} test/ServerTest.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBS})

project(ClientTest)
add_executable(${PROJECT_NAME} test/ClientTest.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBS})

