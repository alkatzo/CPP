cmake_minimum_required(VERSION 3.25.1) # The version of CMake.
project(otel-http-export-demo) # The project name.

add_executable(otel-http-export-demo main.cpp) # The project name and the main file.

find_package(opentelemetry-cpp CONFIG REQUIRED)
find_package(protobuf)
find_package(gRPC)
find_package(CURL)
find_package(nlohmann_json)

get_property(otel_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "Available targets: ${otel_targets}")


include_directories("${OPENTELEMETRY_CPP_INCLUDE_DIRS}")

target_link_libraries(otel-http-export-demo
    opentelemetry_trace
    opentelemetry_common
    opentelemetry_http_client_curl
    opentelemetry_exporter_otlp_http
    opentelemetry_exporter_otlp_grpc
    opentelemetry_exporter_otlp_http_client
    opentelemetry_otlp_recordable
    opentelemetry_resources
)
