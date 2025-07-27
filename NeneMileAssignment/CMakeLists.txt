cmake_minimum_required(VERSION 3.16)

project(NeneMileAssignment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NeneMileAssignment main.cpp)

include(GNUInstallDirs)
install(TARGETS NeneMileAssignment
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
