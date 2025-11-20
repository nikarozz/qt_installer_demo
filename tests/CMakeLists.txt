cmake_minimum_required(VERSION 3.16)

find_package(Qt5 REQUIRED COMPONENTS Test Core Widgets)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(test_package_repository
    test_package_repository.cpp
)
target_link_libraries(test_package_repository
    Qt5::Test
    Qt5::Core
    installer_lib
)
add_test(NAME test_package_repository COMMAND test_package_repository)

add_executable(test_wizard_navigation
    test_wizard_navigation.cpp
)
target_link_libraries(test_wizard_navigation
    Qt5::Test
    Qt5::Core
    installer_lib
)
add_test(NAME test_wizard_navigation COMMAND test_wizard_navigation)
