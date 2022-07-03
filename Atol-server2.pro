QT += quick widgets network svg

CONFIG += c++11
CONFIG += qtquickcompiler
CONFIG += static
LIBS += -lodbc32
# QMAKE_CXXFLAGS += -MP

RC_ICONS += resources/images/appIcon.ico
# QMAKE_LFLAGS_WINDOWS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

SOURCES += \
        sources/atol.cpp \
    sources/core/httpWorker.cpp \
        sources/core/httpserver.cpp \
        sources/core/mewbas.cpp \
        sources/core/network.cpp \
    sources/core/receiptparser.cpp \
    sources/core/recieptlogger.cpp \
    sources/core/reciepts.cpp \
        sources/core/server.cpp \
        sources/core/terminal.cpp \
    sources/core/terminalparser.cpp \
        sources/core/timer.cpp \
        sources/core/tray.cpp \
        sources/core/types/configs.cpp \
    sources/core/types/daemon.cpp \
        sources/core/types/logs.cpp \
        sources/core/types/payment.cpp \
        sources/core/types/request.cpp \
        sources/core/types/task.cpp \
        sources/core/jwt.cpp \
    sources/core/types/tobject.cpp \
        sources/main.cpp \

RESOURCES += resources/qml.qrc

QML_IMPORT_PATH =
QML_DESIGNER_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    sources/atol.h \
    sources/core/httpWorker.h \
    sources/core/httpserver.h \
    sources/core/mewbas.h \
    sources/core/network.h \
    sources/core/receiptparser.h \
    sources/core/recieptlogger.h \
    sources/core/reciepts.h \
    sources/core/server.h \
    sources/core/terminal.h \
    sources/core/terminalparser.h \
    sources/core/timer.h \
    sources/core/tray.h \
    sources/core/types/configs.h \
    sources/core/types/daemon.h \
    sources/core/types/logs.h \
    sources/core/types/payment.h \
    sources/core/types/request.h \
    sources/core/types/task.h \
    sources/core/jwt.h \
    sources/core/types/tobject.h

DISTFILES +=
