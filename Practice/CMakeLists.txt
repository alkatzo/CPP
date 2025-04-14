cmake_minimum_required(VERSION 3.16)

project(ModernCppConcurrencyInDepthUdemy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add sub-projects
add_subdirectory(ThreadManagementGuide)
add_subdirectory(ThreadManagementGuide-accumulate)
add_subdirectory(WorkerActivity)
