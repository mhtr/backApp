CONFIG += c++17 console

QMAKE_CXXFLAGS += /std:c++17

HEADERS += \
    watcher.hpp

SOURCES += \
    main.cpp \
    watcher.cpp

INCLUDEPATH += D:\boost_1_67_0

LIBS += "-LD:/boost_1_67_0/vs2017/x64/lib/"
