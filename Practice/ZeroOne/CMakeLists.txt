cmake_minimum_required(VERSION 3.16)

project(ZeroOne LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ZeroOne main.cpp
    date.h date.cpp
    myuniqueptr.h
    buffer.h buffer.cpp
    feed.h feed.cpp )

include(GNUInstallDirs)
install(TARGETS ZeroOne
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
