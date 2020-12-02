cmake_minimum_required( VERSION 3.1.0 ) 

set(PROJECT_NAME MySudoku)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

FUNCTION(PREPEND var prefix)
   SET(listVar "")
   FOREACH(f ${ARGN})
      LIST(APPEND listVar "${prefix}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

# FindPackages
set(CMAKE_PREFIX_PATH "C:/Qt/5.8/msvc2015_64/lib/cmake/Qt5")
find_package(Qt5 5.8 COMPONENTS Widgets REQUIRED)

add_subdirectory(Code)
add_subdirectory(Tests)
