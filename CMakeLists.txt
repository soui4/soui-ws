cmake_minimum_required(VERSION 3.10)

project(ws)

option(WS_STATIC_CRT "use static CRuntime on Windows (option /MT)" ON)
 
if(WS_STATIC_CRT)
    add_compile_options($<$<CONFIG:Debug>:/MTd>)    # Runtime Library: /MTd = MultiThreaded Debug Runtime
    add_compile_options($<$<CONFIG:Release>:/MT>)   # Runtime Library: /MT  = MultiThreaded Runtime
else()
    add_compile_options($<$<CONFIG:Debug>:/MDd>)    # Runtime Library: /MDd = MultiThreadedDLL Debug Runtime
    add_compile_options($<$<CONFIG:Release>:/MD>)   # Runtime Library: /MD  = MultiThreadedDLL Runtime
endif(WS_STATIC_CRT)

add_definitions(-DHAVE_EX_DATA -DOPENSSL_ALL)
set(LWS_WITH_MINIMAL_EXAMPLES OFF CACHE BOOL "")
set(LWS_WITHOUT_TESTAPPS ON CACHE BOOL "")
set(LWS_WITH_STATIC ON CACHE BOOL "")
set(LWS_WITH_SHARED OFF CACHE BOOL "")
set(LWS_OPENSSL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/openssl_output/x86/include CACHE PATH "")
add_subdirectory(libwebsockets)

set(OPENSSL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/openssl_output/x86/include")
set(LWS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/libwebsockets")
link_directories("${CMAKE_CURRENT_BINARY_DIR}/libwebsockets/lib")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/openssl_output/x64/lib")
else()
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/openssl_output/x86/lib")
endif()

set(LWS_LIB "Crypt32.lib;Ws2_32.lib;websockets_static.lib;libssl.lib;libcrypto.lib")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/openssl-1.1.1/include)
include_directories("$ENV{SOUI4PATH}/config")
include_directories("$ENV{SOUI4PATH}/utilities/include")
add_subdirectory(ws)
