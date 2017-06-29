get_filename_component(exercise ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Basic CMake project
cmake_minimum_required(VERSION 2.8.11)

# Name the project after the exercise
project(${exercise} CXX)

# Locate Boost libraries:, date_time and regex
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set ( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ~/workspace/cpp-netlib-0.12.0-final/build)
find_package(Boost 1.64 REQUIRED COMPONENTS system date_time regex program_options filesystem)
#find_package(CURL)
find_package ( cppnetlib 0.11.0 REQUIRED )
find_package (OpenSSL REQUIRED)
# Enable C++11 features on gcc/clang
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()


# Implementation could be only a header
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${exercise}.cpp)
    set(exercise_cpp ${exercise}.cpp)
else()
    set(exercise_cpp "")
endif()

add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)

# Build executable from sources and headers
add_executable(${exercise}  ${exercise_cpp} search_images.cpp search_images.h )

# We need boost includes
target_include_directories(${exercise} PRIVATE ${Boost_INCLUDE_DIRS})
include_directories ( ${CPPNETLIB_INCLUDE_DIRS} )
include_directories(${OPENSSL_INCLUDE_DIR})


# We need boost libraries
target_link_libraries(${exercise} ${Boost_LIBRARIES})
#target_link_libraries(${exercise} ${CURL_LIBRARIES})
target_link_libraries ( ${exercise} ${CPPNETLIB_LIBRARIES} )
target_link_libraries ( ${exercise} ${OPENSSL_LIBRARIES} )



