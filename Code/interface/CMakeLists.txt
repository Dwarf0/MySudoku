
set(PROJECT_NAME SudokuUI)
set(PROJECT_BIN_DIR C:/Sandbox/Binaries/${PROJECT_NAME})
project(${PROJECT_NAME})

# Files
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
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
    ui/InitDialog.ui
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

target_link_libraries(${PROJECT_NAME} 
    # ${SUDOKU_SOLVER_LIB} 
    SudokuSolver
    Qt5::Widgets
)
add_dependencies(${PROJECT_NAME} SudokuSolver)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_NAME}/$<CONFIG>/
    # PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/include/
    COMPONENT ${PROJECT_NAME}
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${MAIN_PROJECT_NAME}/$<CONFIG>/
    # PUBLIC_HEADER DESTINATION ${MAIN_PROJECT_NAME}/include/
    COMPONENT ${MAIN_PROJECT_NAME}
)


# set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
# set(CPACK_COMPONENTS_ALL ${MAIN_PROJECT_NAME})

# Build directory, Project Name, Project Component, Directory
# set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>;${MAIN_PROJECT_NAME};${MAIN_PROJECT_NAME};/")
# set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}/Code/interface/$<CONFIG>;${MAIN_PROJECT_NAME};${MAIN_PROJECT_NAME};/")
# set(CPACK_SOURCE_INSTALL_CMAKE_PROJECTS )

# include(CPack)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
        COMMAND "windeployqt.exe" --dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" --verbose 0 --no-compiler-runtime --no-translations --no-angle --$<LOWER_CASE:$<CONFIG>> --no-opengl-sw "$<TARGET_FILE:${PROJECT_NAME}>"
    )

# copy deployment directory during installation
install(
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
    DESTINATION ${MAIN_PROJECT_NAME}/$<CONFIG>/
    COMPONENT QT_dependencies
)


# list(TRANSFORM FILES_TO_TRANSLATE PREPEND ${CMAKE_CURRENT_SOURCE_DIR})
