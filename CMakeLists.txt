cmake_minimum_required(VERSION 3.7)

project(conquest)

include_directories(include/)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(conquest ${SOURCE_FILES})
target_compile_features(conquest PRIVATE cxx_std_17)

if(UNIX)
	find_package(Curses REQUIRED)
	target_link_libraries(conquest PRIVATE
		ncurses
	)
endif()