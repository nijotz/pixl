cmake_minimum_required(VERSION 2.8.8)
project(arduino_mock_builder C CXX)
include(ExternalProject)

ExternalProject_Add(arduino_mock
    URL https://github.com/ikeyasu/arduino-mock/archive/0.0.1.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/arduino_mock
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(arduino_mock source_dir)
set(ARDUINO_MOCK_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)

ExternalProject_Get_Property(arduino_mock binary_dir)
set(ARDUINO_MOCK_LIBS_DIR ${binary_dir} PARENT_SCOPE)

