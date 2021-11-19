cmake_minimum_required(VERSION 3.0)

# Setup source files
set(SOURCE_FILES
        Base64.cpp
        Command.cpp
        Config.cpp
        Database.cpp
        DataBuffer.cpp
        File.cpp
        Functions.c
        GenericCommand.cpp
        Hash.cpp
        HTTP_Server.cpp
        INIConfig.cpp
        IRC_Client.cpp
        Jupiter.cpp
        Plugin.cpp
        Rehash.cpp
        SecureSocket.cpp
        Socket.cpp
        TCPSocket.cpp
        Timer.cpp
        UDPSocket.cpp)

# Setup library build target
add_library(jupiter ${SOURCE_FILES})

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(jupiter
        OpenSSL::SSL
        OpenSSL::Crypto
        ${CMAKE_DL_LIBS})

# Setup include directories
target_include_directories(jupiter PUBLIC ../include)
target_include_directories(jupiter PRIVATE ../include/Jupiter)
target_include_directories(jupiter PRIVATE ${OPENSSL_INCLUDE_DIR})

# Define JUPITER_SHARED_LIB when compiling shared lib
get_target_property(jupiter_target_type jupiter TYPE)
if (jupiter_target_type STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(jupiter PUBLIC JUPITER_SHARED_LIB)
endif()

# Setup platform-specific definitions
target_compile_definitions(jupiter PRIVATE ${JUPITER_PRIVATE_DEFS})
