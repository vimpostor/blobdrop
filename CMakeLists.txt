project(blobdrop)
cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND QT_MODULES Core Qml Quick QuickControls2)
find_package(Qt6 6.3 COMPONENTS ${QT_MODULES} REQUIRED)
set(QT_PREFIXED_MODULES ${QT_MODULES})
list(TRANSFORM QT_PREFIXED_MODULES PREPEND "Qt6::")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories("src" "src/Models")

file(GLOB_RECURSE SRCS "src/*.cpp")
file(GLOB_RECURSE HDRS "src/*.hpp")
file(GLOB_RECURSE QMLS RELATIVE "${CMAKE_SOURCE_DIR}" "src/qml/*.qml")

list(APPEND RSRC "icons.qrc")
qt6_add_resources(RESOURCES ${RSRC})

qt_add_executable(${PROJECT_NAME} ${SRCS} ${RESOURCES})

qt_add_qml_module(${PROJECT_NAME} URI "Backend" VERSION 1.0 QML_FILES ${QMLS} SOURCES "src/Models/path_model.cpp" DEPENDENCIES QtQml)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_PREFIXED_MODULES})

# install
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
