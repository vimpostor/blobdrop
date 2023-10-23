set(TESTING_TARGET tests)
file(GLOB_RECURSE TESTING_SRCS "*.cpp")

find_package(Qt6Test REQUIRED)

list(REMOVE_ITEM SRCS "${CMAKE_SOURCE_DIR}/src/main.cpp")
qt_add_executable("${TESTING_TARGET}" ${TESTING_SRCS} ${SRCS})
add_test(NAME "${TESTING_TARGET}" COMMAND "${TESTING_TARGET}")
list(APPEND LINK_LIBS Qt6::Test)
target_link_libraries("${TESTING_TARGET}" PRIVATE ${LINK_LIBS})
