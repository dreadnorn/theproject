QT       += webkitwidgets network widgets
QMAKE_CXXFLAGS += -std=gnu++0x
FORMS     = window.ui
HEADERS   = window.h \
    userinfo.h \
    autosaver.h \
    cookiejar.h
SOURCES   = main.cpp \
            window.cpp \
    autosaver.cpp \
    cookiejar.cpp

TARGET = domTraversal5
INSTALLS += target
