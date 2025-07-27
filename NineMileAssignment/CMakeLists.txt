cmake_minimum_required(VERSION 3.16)

project(NineMileAssignment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NineMileAssignment main.cpp
    recovery.h
    exchange_traits.h
    market_data_message.h)

include(GNUInstallDirs)
install(TARGETS NineMileAssignment
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
