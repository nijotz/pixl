message(STATUS "downloading...
     src='https://github.com/paulsapps/gmock-1.7.0/archive/master.zip'
     dst='/Users/nick/src/pixelmap/test/build/arduino_mock/arduino_mock/src/arduino_mock-build/lib/gmock/gmock/src/master.zip'
     timeout='none'")




file(DOWNLOAD
  "https://github.com/paulsapps/gmock-1.7.0/archive/master.zip"
  "/Users/nick/src/pixelmap/test/build/arduino_mock/arduino_mock/src/arduino_mock-build/lib/gmock/gmock/src/master.zip"
  SHOW_PROGRESS
  # no EXPECTED_HASH
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'https://github.com/paulsapps/gmock-1.7.0/archive/master.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
