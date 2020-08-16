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

# Files
include_directories(${CMAKE_CURRENT_BINARY_DIR} Code/include)
set(SOURCE_FILES
        Code/src/MySudokuViewer.cpp
        Code/src/SudokuModel.cpp
        Code/src/SudokuView.cpp
)
PREPEND(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE_FILES})

set(HEADER_FILES
        Code/include/MySudokuViewer.h
        Code/include/SudokuModel.h
		Code/include/SudokuView.h
		Code/include/globals.h
)
PREPEND(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${HEADER_FILES})

set(FORMS_FILES
        Code/ui/MainWindow.ui
)
PREPEND(FORMS_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${FORMS_FILES})


set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
# qt5_add_resources(UI_RESOURCES Code/resources/resources.qrc)
qt5_wrap_cpp(MOC_HEADERS ${HEADER_FILES})
qt5_wrap_ui(FORMS_HEADERS ${FORMS_FILES})

set(ALL_FILES 
        ${SOURCE_FILES} 
        ${HEADER_FILES}
        ${UI_RESOURCES}
        ${MOC_HEADERS}
        ${FORMS_HEADERS}
)

add_executable(${PROJECT_NAME} ${ALL_FILES} Code/src/main.cpp)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

# list(TRANSFORM FILES_TO_TRANSLATE PREPEND ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Tests)
