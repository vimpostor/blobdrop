cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0048 NEW)
project(blobdrop VERSION 2.1 DESCRIPTION "Drag and drop files directly out of the terminal")

option(BUILD_TESTING "Build the testing tree.")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND QT_MODULES Core Qml Quick QuickControls2 DBus Svg)
find_package(Qt6 6.7 COMPONENTS ${QT_MODULES} REQUIRED)
qt_standard_project_setup()
list(TRANSFORM QT_MODULES PREPEND "Qt6::")

list(APPEND LINK_LIBS ${QT_MODULES})

if (UNIX AND NOT APPLE)
	find_package(PkgConfig REQUIRED)
	list(APPEND PKGCONFIG_MODULES "xcb" "xcb-ewmh")
	foreach(PKG IN LISTS PKGCONFIG_MODULES)
		pkg_check_modules("${PKG}" REQUIRED IMPORTED_TARGET "${PKG}")
	endforeach()
	list(TRANSFORM PKGCONFIG_MODULES PREPEND "PkgConfig::")
endif()

include(FetchContent)
FetchContent_Declare(quartz GIT_REPOSITORY https://github.com/vimpostor/quartz.git GIT_TAG v0.5)
FetchContent_MakeAvailable(quartz)

list(APPEND LINK_LIBS ${PKGCONFIG_MODULES})

include_directories("src" "src/Models")
add_compile_definitions(BLOBDROP_VERSION="${PROJECT_VERSION}")

file(GLOB_RECURSE SRCS "src/*.cpp")
file(GLOB_RECURSE HDRS "src/*.hpp")
file(GLOB_RECURSE QMLS RELATIVE "${CMAKE_SOURCE_DIR}" "src/qml/*.qml")

list(APPEND RSRC "icons.qrc")
qt_add_resources(RESOURCES ${RSRC})

qt_add_executable(${PROJECT_NAME} ${SRCS} ${RESOURCES})

qt_add_qml_module(${PROJECT_NAME} URI "Backend" VERSION "${PROJECT_VERSION}" NO_RESOURCE_TARGET_PATH QML_FILES ${QMLS} SOURCES "src/Models/path_model.cpp")
set_target_properties(${PROJECT_NAME} PROPERTIES QT_QMLCACHEGEN_ARGUMENTS "--only-bytecode")

target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBS})
quartz_link(${PROJECT_NAME} NO_ICONS)

# install
install(TARGETS ${PROJECT_NAME} RUNTIME)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/doc/man/" TYPE MAN)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets/completions/" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}")

# testing
if (BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()
