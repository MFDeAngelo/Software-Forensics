cmake_minimum_required(VERSION 3.15.3)
project(CipherTechChallenges)


set(FILES_1
	Challenge1.cpp
	LFSR.hpp
	LFSR.cpp)

set(FILES_2
	Challenge2.cpp
	LFSR.hpp
	LFSR.cpp
	KDB.hpp
	KDB.cpp)

set(FILES_3
	Challenge3.cpp
	LFSR.hpp
	LFSR.cpp
	KDB.hpp
	KDB.cpp
	MD5.hpp
	MD5.cpp)


add_executable(LFSR ${FILES_1})
set_property(TARGET LFSR PROPERTY CXX_STANDARD 17)

add_executable(KDB ${FILES_2})
set_property(TARGET KDB PROPERTY CXX_STANDARD 17)

add_executable(JPEG ${FILES_3})
set_property(TARGET JPEG PROPERTY CXX_STANDARD 17)

set(EXECUTABLES
	LFSR
	KDB
	JPEG)


foreach(EXECUTABLE ${EXECUTABLES})
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		target_compile_options(${EXECUTABLE} PRIVATE /W4 /permissive-)
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra -pedantic)
	endif()
endforeach()
