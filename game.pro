QT       += core gui widgets
CONFIG   += c++17 console
SOURCES  += Main_Menu.cpp \
            time_loop.cpp
HEADERS  +=
LIBS     += -lraylib -lm -ldl -lGL -lpthread -lrt -lX11
