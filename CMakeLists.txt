cmake_minimum_required(VERSION 3.1)

project( IN54_Project )

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE source_files src/*)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( IN54_Project ${source_files} )

target_link_libraries( IN54_Project ${OpenCV_LIBS} )
