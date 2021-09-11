include_directories("${CMAKE_SOURCE_DIR}/include")

set(SOURCES threadpool.c
            circularqueue/circularqueue.c)

add_library (threadpool STATIC ${SOURCES})
target_link_libraries(threadpool PRIVATE Threads::Threads)

# target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
