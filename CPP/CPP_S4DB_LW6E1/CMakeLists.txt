cmake_minimum_required(VERSION 3.5)

project(CPP_S4DB_LW6E1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS sql)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        postgreconnection.cpp
        postgreconnection.h
        dbconnectionsettingsdialog.h
        dbconnectionsettingsdialog.cpp
        connectiondata.h
        connectiondata.cpp
        querydialog.h
        querydialog.cpp
        insertquerydialog.h
        insertquerydialog.cpp
        updatequerydialog.h
        updatequerydialog.cpp
        deletequerydialog.h
        deletequerydialog.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CPP_S4DB_LW6E1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CPP_S4DB_LW6E1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CPP_S4DB_LW6E1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CPP_S4DB_LW6E1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CPP_S4DB_LW6E1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CPP_S4DB_LW6E1 PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(CPP_S4DB_LW6E1 PRIVATE Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(CPP_S4DB_LW6E1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CPP_S4DB_LW6E1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CPP_S4DB_LW6E1)
endif()
