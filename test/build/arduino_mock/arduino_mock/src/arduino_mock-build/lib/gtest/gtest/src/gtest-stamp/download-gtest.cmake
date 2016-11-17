message(STATUS "downloading...
     src='http://pkgs.fedoraproject.org/repo/pkgs/gtest/gtest-1.7.0.zip/2d6ec8ccdf5c46b05ba54a9fd1d130d7/gtest-1.7.0.zip'
     dst='/Users/nick/src/pixelmap/test/build/arduino_mock/arduino_mock/src/arduino_mock-build/lib/gtest/gtest/src/gtest-1.7.0.zip'
     timeout='none'")




file(DOWNLOAD
  "http://pkgs.fedoraproject.org/repo/pkgs/gtest/gtest-1.7.0.zip/2d6ec8ccdf5c46b05ba54a9fd1d130d7/gtest-1.7.0.zip"
  "/Users/nick/src/pixelmap/test/build/arduino_mock/arduino_mock/src/arduino_mock-build/lib/gtest/gtest/src/gtest-1.7.0.zip"
  SHOW_PROGRESS
  # no EXPECTED_HASH
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'http://pkgs.fedoraproject.org/repo/pkgs/gtest/gtest-1.7.0.zip/2d6ec8ccdf5c46b05ba54a9fd1d130d7/gtest-1.7.0.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
