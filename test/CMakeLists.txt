cmake_minimum_required (VERSION 2.8.8)
project (arduino_mock_sample)

add_subdirectory(arduino-mock)

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock/include
    lib/
)

file(GLOB SRC "test.cpp")
add_executable(arduino_mock_sample test.cpp)

target_link_libraries(arduino_mock_sample
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/libgtest.a
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock-build/libgmock.a
    ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(arduino_mock_sample arduino-mock)

enable_testing()
add_test(arduino_mock_sample_1 arduino_mock_sample)
