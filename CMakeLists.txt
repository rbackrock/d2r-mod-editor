cmake_minimum_required(VERSION 3.29.9)
project(D2rModEditor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# 一般性配置
set(TARGET_NAME d2rModEditor)
set(SDLTTF_VENDORED ON)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(
  ${TARGET_NAME}
)

file(
  GLOB_RECURSE
  SOURCE_FILES
  "src/*.cpp",
  "vendored/imgui/*.cpp"
)
target_sources(
  ${TARGET_NAME} PRIVATE
  ${SOURCE_FILES}
)

target_link_libraries(
  ${TARGET_NAME} PRIVATE
  SDL3::SDL3
)

target_include_directories(
  ${TARGET_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendored/imgui/include
)