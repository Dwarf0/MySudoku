
find_package(GTest REQUIRED)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ./include
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

add_definitions( -DTESTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}" )

set(TEST_FILES
    src/tests.cpp
    src/main.cpp
)

add_executable(SudokuTests ${TEST_FILES} ${SOURCE_FILES} ${HEADER_FILES} )
target_link_libraries(SudokuTests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Qt5::Widgets)
# target_link_libraries(SudokuTests GTest::GTest GTest::Main)

gtest_add_tests(
    TARGET      SudokuTests
    # TEST_SUFFIX .noArgs
    # TEST_LIST   noArgsTests
)

# set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)
