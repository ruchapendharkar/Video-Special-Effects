cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
#add_executable( DisplayImage imgDisplay.cpp)
add_executable(vidDisplay vidDisplay.cpp)
#add_executable(timeBlur timeBlur.cpp)
target_link_libraries( vidDisplay ${OpenCV_LIBS} )
