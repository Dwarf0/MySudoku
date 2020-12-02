

# Files
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_SOURCE_DIR}/Code/global_include 
    ${CMAKE_SOURCE_DIR}/Code/solver/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SOURCE_FILES
    src/MySudokuViewer.cpp
    src/SudokuModel.cpp
    src/SudokuView.cpp
    src/SudokuDelegate.cpp
)
PREPEND(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE_FILES})

set(HEADER_FILES
    include/MySudokuViewer.h
    include/SudokuModel.h
    include/SudokuView.h
    include/SudokuDelegate.h
)
PREPEND(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${HEADER_FILES})

set(FORMS_FILES
    ui/MainWindow.ui
)
PREPEND(FORMS_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${FORMS_FILES})


set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
# qt5_add_resources(UI_RESOURCES resources/resources.qrc)
qt5_wrap_cpp(MOC_HEADERS ${HEADER_FILES})
qt5_wrap_ui(FORMS_HEADERS ${FORMS_FILES})

set(ALL_FILES 
    ${SOURCE_FILES} 
    ${HEADER_FILES}
    ${UI_RESOURCES}
    ${MOC_HEADERS}
    ${FORMS_HEADERS}
)

add_executable(${PROJECT_NAME} ${ALL_FILES} src/main.cpp)

target_link_libraries(${PROJECT_NAME} SudokuSolver Qt5::Widgets)

# list(TRANSFORM FILES_TO_TRANSLATE PREPEND ${CMAKE_CURRENT_SOURCE_DIR})
