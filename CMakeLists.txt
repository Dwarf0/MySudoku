cmake_minimum_required(VERSION 3.1.0)

set(PROJECT_NAME MySudoku)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

# FindPackages
set(CMAKE_PREFIX_PATH "C:/Qt/5.8/msvc2015_64/lib/cmake/Qt5")
find_package(Qt5 5.8 COMPONENTS Widgets REQUIRED)

# Files
include_directories(${CMAKE_CURRENT_BINARY_DIR} Code/include)
set(SOURCE_FILES
        Code/src/main.cpp
        Code/src/MySudokuViewer.cpp
        Code/src/SudokuModel.cpp
)
set(HEADER_FILES
        Code/include/MySudokuViewer.h
        Code/include/SudokuModel.h
)
set(FORMS_FILES
        # Code/ui/TranslatorGroupBox.ui
		# Code/ui/GrammarRulesGroupBox.ui
        Code/ui/MainWindow.ui
)

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

add_executable(${PROJECT_NAME} ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
