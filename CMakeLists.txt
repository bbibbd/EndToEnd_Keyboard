cmake_minimum_required(VERSION 3.0.0)
project(trt_interface_app VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-write-strings")	

find_package(CUDA REQUIRED)
message("-- CUDA version: ${CUDA_VERSION}")

find_package(OpenCV REQUIRED)

set(PROJECT_OUTPUT_DIR  ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
set(PROJECT_INCLUDE_DIR ${PROJECT_OUTPUT_DIR}/include)

file(MAKE_DIRECTORY ${PROJECT_INCLUDE_DIR})
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)

message("-- system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
message("-- output path:  ${PROJECT_OUTPUT_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)


# build C/C++ library
include_directories(
    ${PROJECT_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_INCLUDE_DIR}/jetson-utils
    ${CUDA_INCLUDE_DIRS}
)
# build C/C++ interface

add_subdirectory(utils)

set(DEPEND_LIBS
    jetson-utils
    nvinfer
    nvinfer_plugin
    nvcaffe_parser
    nvonnxparser
    ${OpenCV_LIBS}
)

add_executable(trt_interface_app src/main.cpp
    src/randInt8Calibrator.cpp
    src/tensorNet.cpp
    src/etoeNet.cpp
    src/keyboard_driving.cpp
    src/autonomous_driving.cpp
    )

target_link_libraries(trt_interface_app ${DEPEND_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
