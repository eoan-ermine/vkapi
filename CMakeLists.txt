cmake_minimum_required(VERSION 3.9)
project(VkApi VERSION 1.0.1)

add_library(VkApi SHARED sources/VkApi.cpp)
include(GNUInstallDirs)
set_target_properties(VkApi PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VkApi PROPERTIES PUBLIC_HEADER include/VkApi.h)
target_include_directories(VkApi PRIVATE .)
install(TARGETS VkApi
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})