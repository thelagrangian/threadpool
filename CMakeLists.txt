cmake_minimum_required(VERSION 3.1.0)
project(threadpool)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include)
 
set(SOURCES main.c
            src/threadpool.c
            src/circularqueue/circularqueue.c)
 
# file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "src/*.c")
 
add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE Threads::Threads)
