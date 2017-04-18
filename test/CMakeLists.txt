cmake_minimum_required (VERSION 2.8.8)
project (pixl_test)

add_subdirectory(arduino-mock)
add_subdirectory(fastled)

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock/include
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/
)

file(GLOB SRC "test.cpp")
add_executable(pixl_test test.cpp)

target_link_libraries(pixl_test
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/libgtest.a
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock-build/libgmock.a
    ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(pixl_test arduino-mock)
add_dependencies(pixl_test fastled)

enable_testing()
add_test(arduino_mock_sample_1 pixl_test)
