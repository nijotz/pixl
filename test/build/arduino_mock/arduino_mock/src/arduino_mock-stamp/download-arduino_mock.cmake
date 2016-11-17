message(STATUS "downloading...
     src='https://github.com/ikeyasu/arduino-mock/archive/0.0.1.zip'
     dst='/Users/nick/src/pixelmap/test/build/arduino_mock/arduino_mock/src/0.0.1.zip'
     timeout='none'")




file(DOWNLOAD
  "https://github.com/ikeyasu/arduino-mock/archive/0.0.1.zip"
  "/Users/nick/src/pixelmap/test/build/arduino_mock/arduino_mock/src/0.0.1.zip"
  SHOW_PROGRESS
  # no EXPECTED_HASH
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'https://github.com/ikeyasu/arduino-mock/archive/0.0.1.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
