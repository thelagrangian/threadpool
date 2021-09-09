cmake_minimum_required(VERSION 3.1.0)
project(threadpool)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#Bring the headers, such as Student.h into the project
include_directories(include)
 
set(SOURCES main.c
            src/threadpool.c)
 
# However, the file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "src/*.cpp")
 
add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE Threads::Threads)
