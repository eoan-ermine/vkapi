cmake_minimum_required(VERSION 2.8.3)
project(main)

set(Poco_DIR "/usr/local/lib/cmake/Poco/") 
set(Poco_INCLUDE_DIRS "/usr/include/Poco/")

find_package(Poco REQUIRED COMPONENTS Foundation Net XML Util Crypto NetSSL) # add other components here

# check c++11 / c++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11 " COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories( ${Poco_INCLUDE_DIRS})

add_executable(main main.cpp)
target_link_libraries(main ${Poco_LIBRARIES}) 