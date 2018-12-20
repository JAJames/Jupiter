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
add_library(jupiter SHARED ${SOURCE_FILES})

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Setup include directories
target_include_directories(jupiter PUBLIC ../include)
target_include_directories(jupiter PRIVATE ../include/Jupiter)
target_include_directories(jupiter PRIVATE ${OPENSSL_INCLUDE_DIR})

# Setup platform-specific definitions
target_compile_definitions(jupiter PRIVATE ${JUPITER_PRIVATE_DEFS})