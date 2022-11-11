project(capdump)
cmake_minimum_required(VERSION 3.7)

option(WITH_TESTS "Build tests" OFF)
set(EXAMPLES_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples)

add_library(capdump
    capdump.c
)
set(CMAKE_BUILD_TYPE "Debug")
set(CXX_FLAGS
    -g
    # -DVALGRIND
    # -DMUDUO_STD_STRING
    #-DCHECK_PTHREAD_RETURN_VALUE
    #-D_FILE_OFFSET_BITS=64
    -Wall
    #-Wextra
    -Werror
    #-Wconversion
    #-Wno-unused-parameter
    #-Wold-style-cast
    #-Woverloaded-virtual
    #-Wpointer-arith
    #-Wshadow
    #-Wwrite-strings
    #-march=native
    # -MMD
    # -std=c++0x
    #-std=c++11
    #-rdynamic
)

# set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_CXX_FLAGS_DEBUG "-O0")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

target_include_directories(capdump
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_compile_options(capdump
    PRIVATE
    -Wall
    -fPIC
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

target_link_libraries(capdump
    -lpcap
    -lpthread
    -lm
)

install(TARGETS capdump EXPORT cptTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include)

add_executable(cptbin examples/capture.c)
target_link_libraries(cptbin capdump)

set_target_properties(capdump cptbin PROPERTIES
    CXX_STANDARD 11
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_DIR}
)

file(MAKE_DIRECTORY ${EXAMPLES_DIR})

if (WITH_TESTS)
    
endif()
