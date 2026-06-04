cmake_minimum_required(VERSION 3.5)

project(SmartStorage VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED QT_VERSION_MAJOR)
    set(QT_VERSION_MAJOR 6)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    Charts
    Svg
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.h        mainwindow.cpp
    splashscreen.h      splashscreen.cpp
    appstyle.h

    models/product.h
    models/employee.h
    models/warehousecell.h
    models/activitylog.h

    pages/dashboardpage.h       pages/dashboardpage.cpp
    pages/itemspage.h           pages/itemspage.cpp
    pages/itemcardpage.h        pages/itemcardpage.cpp
    pages/employeespage.h       pages/employeespage.cpp
    pages/warehousemappage.h    pages/warehousemappage.cpp
)

add_executable(SmartStorage ${PROJECT_SOURCES})

target_link_libraries(SmartStorage PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Svg
)

target_include_directories(SmartStorage PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/pages
)

if(WIN32)
    set_target_properties(SmartStorage PROPERTIES
        WIN32_EXECUTABLE $<CONFIG:Release>
    )
endif()

message(STATUS "Qt version  : ${Qt${QT_VERSION_MAJOR}_VERSION}")