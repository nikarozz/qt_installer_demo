cmake_minimum_required(VERSION 3.16)

project(QtInstallerDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

option(ENABLE_TESTS "Build tests" ON)

set(INSTALLER_LIB_SOURCES
    src/main_window.cpp
    src/intro_widget.cpp
    src/package_selection_widget.cpp
    src/package_repository.cpp
    src/installer_service.cpp
    src/resource_extractor.cpp
    src/default_process_runner.cpp
    src/wizard_navigation.cpp
)

add_library(installer_lib STATIC
    ${INSTALLER_LIB_SOURCES}
)

target_include_directories(installer_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(installer_lib
    PRIVATE
        Qt5::Core
        Qt5::Widgets
)

add_executable(installer_app
    src/main.cpp
    resources/packages.qrc
)

target_link_libraries(installer_app
    PRIVATE
        installer_lib
        Qt5::Core
        Qt5::Widgets
)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
