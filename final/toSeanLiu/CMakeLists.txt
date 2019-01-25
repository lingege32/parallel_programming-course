project(ppFinal CXX)

cmake_minimum_required(VERSION 3.0)

# setting PATH
include(GNUInstallDirs)

set(CMAKE_SOURCE_DIR .)
#set(CMAKE_CXX_COMPILER "g++")
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")
set(EXECUTABLE_OUTPUT_PATH .)
#where are the include files
include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)
add_executable(${CMAKE_PROJECT_NAME} ${SRC})

MESSAGE( STATUS "CMAKE_INSTALL_LIBDIR:  " ${CMAKE_INSTALL_LIBDIR} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_DIR:  " ${CMAKE_CURRENT_LIST_DIR} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS:  " ${CMAKE_CXX_FLAGS} )
# using C++11
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
# not allow -std=gnu++11
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)
# set compile option
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall  -Werror ${CMAKE_CXX_FLAGS})
