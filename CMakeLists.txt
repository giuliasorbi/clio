get_filename_component(exercise ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Basic CMake project
cmake_minimum_required(VERSION 2.8.11)

# Name the project after the exercise
project(${exercise} CXX)

# Locate Boost libraries:, date_time and regex
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.59 REQUIRED COMPONENTS system date_time regex)
find_package(CURL)

# Enable C++11 features on gcc/clang
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()


# Get a source filename from the exercise name by replacing -'s with _'s

# Implementation could be only a header
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${exercise}.cpp)
    set(exercise_cpp ${exercise}.cpp)
else()
    set(exercise_cpp "")
endif()

# Build executable from sources and headers
add_executable(${exercise}  ${exercise_cpp} utils.cpp utils.h ) # ${file}.h)

# We need boost includes
target_include_directories(${exercise} PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${exercise} PRIVATE  ${CURL_INCLUDE_DIR})

# We need boost libraries
target_link_libraries(${exercise} ${Boost_LIBRARIES})
target_link_libraries(${exercise} ${CURL_LIBRARIES})


