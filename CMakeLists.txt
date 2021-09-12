cmake_minimum_required(VERSION 3.1.0)
project(threadpool)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

# include_directories(include)
 
# file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "src/*.c")

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib ) # static library
# set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib ) # shared library

# add_executable(main ${SOURCES})
# target_link_libraries(main PRIVATE Threads::Threads)

# add_library(threadpool STATIC ${SOURCES})
# target_link_libraries(threadpool PRIVATE Threads::Threads)

add_subdirectory(src)
add_subdirectory(test)
