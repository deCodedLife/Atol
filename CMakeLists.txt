cmake_minimum_required(VERSION 3.14)

project(AtolServer VERSION 2.4 LANGUAGES CXX)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set( PROJECT_NAME AtolServer )
find_package(
    Qt6 6.4 REQUIRED
    COMPONENTS
    Quick
    Network
    HttpServer
    SVG
    Widgets
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/tobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/tobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/configs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/configs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/daemon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/daemon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/logs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/logs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/payment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/payment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/request.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/request.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/task.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/types/task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/network.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/network.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/timer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/httpserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/httpserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/httpWorker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/httpWorker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/jwt.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/jwt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/mewbas.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/mewbas.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/receiptparser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/receiptparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/reciepts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/reciepts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/recieptlogger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/recieptlogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/terminal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/terminal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/terminalparser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/terminalparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/tray.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/tray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/atol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/atol.h
)

qt_standard_project_setup()
qt_add_executable(
    AtolServer
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/Atol.rc
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/qml.qrc
)

set( PROJECT_NAME AtolServer )

set_target_properties(
    AtolServer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER me.codedlife.atol
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(
    AtolServer
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Network
    PRIVATE Qt6::HttpServer
    PRIVATE Qt6::Widgets
)

#set( UPX_EXE C:/tools/upx.exe )
#set( BUILD ${CMAKE_BUILD_RPATH}${PROJECT_NAME}.exe)

set(CPACK_GENERATOR "ZIP")
include(CPack)
