
#add_files(HDRS . *.h)
add_files(SRCS . list_swap.C)

message(STATUS "SRCS: ${SRCS}, HDRS: ${HDRS}")

add_library(Chapter04 ${HDRS} ${SRCS})
add_executable(Chapter04_exe list_swap.C)

target_include_directories(Chapter04 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_subdirectory(test)
