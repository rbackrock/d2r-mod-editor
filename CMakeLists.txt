cmake_minimum_required(VERSION 3.29.9)
project(D2rModEditor)

set(TARGET_NAME d2rModEditor)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SDLTTF_VENDORED ON)
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(vendored/nlohmann_json EXCLUDE_FROM_ALL)

add_custom_target(copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCES_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources"
  COMMENT "Copying resources directory to build directory"
)

add_executable(
  ${TARGET_NAME}
)

# 开发环境添加 -DDEBUG_MODE
target_compile_definitions(
  ${TARGET_NAME}
  PRIVATE
  "$<$<CONFIG:Debug>:DEBUG_MODE>"
)

file(
  GLOB_RECURSE
  SOURCE_FILES
  "src/*.cpp",
  "vendored/imgui/*.cpp",
  "vendored/sqlite/*.c",
)
target_sources(
  ${TARGET_NAME} PRIVATE
  ${SOURCE_FILES}
)

target_link_libraries(
  ${TARGET_NAME} PRIVATE
  SDL3::SDL3
  spdlog::spdlog
  nlohmann_json::nlohmann_json
)

target_include_directories(
  ${TARGET_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendored/imgui/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendored/sqlite/include
)