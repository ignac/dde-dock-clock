QT              += widgets svg
TEMPLATE         = lib
CONFIG          += plugin c++11

TARGET           = clock
DISTFILES       += datetime.json

HEADERS += \
    calendar.h \
    calendarwidget.h \
    datetimeplugin.h \
    datetimewidget.h \
    datewidget.h


SOURCES += \
    calendar.cpp \
    calendarwidget.cpp \
    datetimeplugin.cpp \
    datetimewidget.cpp \
    datewidget.cpp

RESOURCES += resources.qrc
