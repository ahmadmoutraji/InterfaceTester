cmake_minimum_required(VERSION 3.16)

project(InterfaceTester VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core QuickControls2 Network)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appInterfaceTester
    app/main.cpp
    core/TestManager.cpp
    core/TestManager.h
    core/Logger.cpp
    core/Logger.h
    core/NetworkInfo.cpp
    core/NetworkInfo.h
    core/ITestCase.h
    tests/DisplayTestCase.cpp
    tests/DisplayTestCase.h
    tests/LanTestCase.cpp
    tests/LanTestCase.h
    tests/TemperatureTestCase.cpp
    tests/TemperatureTestCase.h
    tests/StubTestCase.cpp
    tests/StubTestCase.h
)

qt_add_qml_module(appInterfaceTester
    URI InterfaceTester
    QML_FILES
        app/qml/Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appInterfaceTester PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appInterfaceTester
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appInterfaceTester
    PRIVATE Qt6::Quick  Qt6::Core Qt6::QuickControls2 Qt6::Network
)

target_include_directories(appInterfaceTester
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

include(GNUInstallDirs)
install(TARGETS appInterfaceTester
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
