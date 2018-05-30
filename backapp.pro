CONFIG += c++1z

QMAKE_CXXFLAGS += -std=c++17

HEADERS += \
    watcher.hpp

SOURCES += \
    main.cpp \
    watcher.cpp


LIBS += -lboost_system -lboost_filesystem -lstdc++fs
