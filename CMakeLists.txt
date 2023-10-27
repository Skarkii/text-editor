cmake_minimum_required(VERSION 3.22)
project(txx)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        src/main.cpp
	src/window.cpp
	src/windowcontroller.cpp
	src/input.cpp
	src/buffer.cpp
	src/color.cpp
)

set(HEADERS
	src/window.h
	src/windowcontroller.h
	src/input.h
	src/buffer.h
	src/color.h
)

set(CURSES_NEED_WIDE TRUE)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE src include)

if (CMAKE_CXX_COPMPILER_ID MATCHES "GNU|Clang")
   target_compile_options(${PROJECT_NAME} PRIVATE -lncursesw -Wall -Wextra)
endif()

target_link_libraries(${PROJECT_NAME}  ncursesw)
