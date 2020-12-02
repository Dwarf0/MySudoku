
# Files
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_SOURCE_DIR}/Code/global_include 
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
    ${FORMS_HEADERS}
)

# add_definitions( -DTESTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}" )

add_library(SudokuSolver SHARED ${ALL_FILES})

target_link_libraries(SudokuSolver Qt5::Widgets)


