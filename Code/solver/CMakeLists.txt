
set(PROJECT_NAME SudokuSolver)
set(PROJECT_BIN_DIR C:/Sandbox/Binaries/${PROJECT_NAME})
project(${PROJECT_NAME})

# Files
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SUDOKU_SOLVER_SOURCE_FILES
    src/SudokuCell.cpp
    src/Sudoku.cpp
    src/solver.cpp
)
# PREPEND(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${SUDOKU_SOLVER_SOURCE_FILES})

set(SUDOKU_SOLVER_HEADER_FILES
    include/SudokuCell.h
    include/Sudoku.h
    include/solver.h
)
# PREPEND(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${SUDOKU_SOLVER_HEADER_FILES})


set(CMAKE_AUTOMOC OFF)
qt5_wrap_cpp(MOC_HEADERS ${SUDOKU_SOLVER_HEADER_FILES})

set(ALL_FILES 
    ${SUDOKU_SOLVER_SOURCE_FILES} 
    ${SUDOKU_SOLVER_HEADER_FILES}
    ${MOC_HEADERS}
)

# add_definitions( -DTESTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}" )

add_library(SudokuSolver ${ALL_FILES})

target_link_libraries(SudokuSolver Qt5::Widgets)

# install(TARGETS SudokuSolver
        # RUNTIME DESTINATION ${PROJECT_BIN_DIR}/$<CONFIG>/
        # PUBLIC_HEADER DESTINATION ${PROJECT_BIN_DIR}/include/
       # )

add_custom_command(TARGET SudokuSolver POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory -p ${PROJECT_BIN_DIR}/include/
    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_BIN_DIR}/${CMAKE_BUILD_TYPE}/include/
    COMMAND "${CMAKE_COMMAND}" -E make_directory -p ${PROJECT_BIN_DIR}/$<CONFIG>/
    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG> ${PROJECT_BIN_DIR}/$<CONFIG>/
)

add_subdirectory(Tests)


