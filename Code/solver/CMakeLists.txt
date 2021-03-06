
set(PROJECT_NAME SudokuSolver)
project(${PROJECT_NAME})

# Files
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SUDOKU_SOLVER_SOURCE_FILES
    src/SudokuCell.cpp
    src/Sudoku.cpp
    src/Solver.cpp
    src/core.cpp
)
# file(GLOB SUDOKU_SOLVER_SOURCE_FILES "src/*.cpp")

set(SUDOKU_SOLVER_PUBLIC_HEADER_FILES
    include/Sudoku.h
    include/Solver.h
    include/Filter.h
)

set(SUDOKU_SOLVER_PRIVATE_HEADER_FILES
    include/SudokuCell.h
    include/core.h
)
# file(GLOB SUDOKU_SOLVER_PRIVATE_HEADER_FILES "include/*.h")

set(CMAKE_AUTOMOC OFF)
qt5_wrap_cpp(MOC_HEADERS ${SUDOKU_SOLVER_HEADER_FILES})

set(ALL_FILES 
    ${SUDOKU_SOLVER_SOURCE_FILES} 
    ${SUDOKU_SOLVER_PUBLIC_HEADER_FILES}
    ${SUDOKU_SOLVER_PRIVATE_HEADER_FILES}
    ${MOC_HEADERS}
)

# add_definitions( -DTESTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}" )

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
list(JOIN SUDOKU_SOLVER_PUBLIC_HEADER_FILES ";" PUBLIC_H)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    PUBLIC_HEADER "${PUBLIC_H}"
    DEBUG_POSTFIX "d"
)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_NAME}/$<CONFIG>/
    PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/include/
    COMPONENT ${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${MAIN_PROJECT_NAME}/$<CONFIG>/
    PUBLIC_HEADER DESTINATION ${MAIN_PROJECT_NAME}/include/
    COMPONENT ${MAIN_PROJECT_NAME}
)

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # COMMAND "${CMAKE_COMMAND}" -E make_directory -p ${PROJECT_BIN_DIR}/include/
    # COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_BIN_DIR}/${CMAKE_BUILD_TYPE}/include/
    # COMMAND "${CMAKE_COMMAND}" -E make_directory -p ${PROJECT_BIN_DIR}/$<CONFIG>/
    # COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG> ${PROJECT_BIN_DIR}/$<CONFIG>/
# )

add_subdirectory(Tests)


