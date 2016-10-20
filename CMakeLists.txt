cmake_minimum_required (VERSION 2.8)
project (PolygonFit)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")

# Copy images
file(GLOB POLYGON_IMAGES "${CMAKE_SOURCE_DIR}/imgs/*")
make_directory("${CMAKE_BINARY_DIR}/imgs")
foreach(DATA_FILE ${POLYGON_IMAGES})
	configure_file(${DATA_FILE} "${CMAKE_BINARY_DIR}/imgs/" COPYONLY)
endforeach(DATA_FILE)

# PolygonFit source
file(GLOB_RECURSE POLYGON_FIT_SRC
	"${CMAKE_SOURCE_DIR}/src/*.h"
	"${CMAKE_SOURCE_DIR}/src/*.cpp")

# Final target
add_executable(PolygonFit ${POLYGON_FIT_SRC})

# OpenCV
find_package(OpenCV REQUIRED core highgui imgproc)
if(OPENCV_FOUND)
	include_directories(${OpenCV_INCLUDE_DIRS})
	link_directories(${OpenCV_LIB_DIR})
	target_link_libraries(PolygonFit ${OpenCV_LIBS})
endif()
