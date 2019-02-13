#-------------------------------------------------
#
# Project created by QtCreator 2018-11-30T14:52:23
#
#-------------------------------------------------

QT       += testlib \
            widgets

TARGET = tst_uitest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage


SOURCES += \
        tst_uitest.cpp \
        ../../../UI/gameboard.cpp \
        ../../../GameLogic/Engine/actorfactory.cpp \
        ../../../GameLogic/Engine/hex.cpp \
        ../../../GameLogic/Engine/piecefactory.cpp \
        ../../../GameLogic/Engine/ioexception.cpp \
        ../../../GameLogic/Engine/formatexception.cpp \
        ../../../GameLogic/Engine/gameexception.cpp \
        ../../../GameLogic/Engine/pawn.cpp \
        ../../../GameLogic/Engine/transport.cpp \
        ../../../GameLogic/Engine/dolphin.cpp \
        ../../../GameLogic/Engine/boat.cpp \
        ../../../GameLogic/Engine/actor.cpp \
        ../../../GameLogic/Engine/kraken.cpp \
        ../../../GameLogic/Engine/seamunster.cpp \
        ../../../GameLogic/Engine/shark.cpp \
        ../../../GameLogic/Engine/vortex.cpp \
        ../../../UI/customview.cpp \
        ../../../UI/gamestate.cpp \
        ../../../UI/mainwindow.cpp \
        ../../../UI/myhexitem.cpp \
        ../../../UI/player.cpp



HEADERS += \
        ../../../GameLogic/Engine/igameboard.hh \
        ../../../UI/gameboard.hh \
        ../../../GameLogic/Engine/igamestate.hh \
        ../../../UI/gamestate.hh \
        ../../../GameLogic/Engine/piecefactory.hh \
        ../../../GameLogic/Engine/hex.hh \
        ../../../GameLogic/Engine/actorfactory.hh \
        ../../../GameLogic/Engine/ioexception.hh \
        ../../../GameLogic/Engine/formatexception.hh \
        ../../../GameLogic/Engine/gameexception.hh \
        ../../../GameLogic/Engine/pawn.hh \
        ../../../GameLogic/Engine/transport.hh \
        ../../../GameLogic/Engine/dolphin.hh \
        ../../../GameLogic/Engine/boat.hh \
        ../../../GameLogic/Engine/actor.hh \
        ../../../GameLogic/Engine/kraken.hh \
        ../../../GameLogic/Engine/seamunster.hh \
        ../../../GameLogic/Engine/shark.hh \
        ../../../GameLogic/Engine/vortex.hh \
        ../../../UI/customview.hh \
        ../../../UI/mainwindow.hh \
        ../../../UI/myhexitem.hh \
        ../../../UI/player.hh \
        ../../../UI/wheelitem.hh

#include "cubecoordinate.hh"
#include "hex.hh"
#include "igamerunner.hh"
#include "gameboard.hh"
#include "gamestate.hh"
#include "player.hh"

#include "actor.hh"
#include "myhexitem.hh"
#include "drawhexobject.hh"
#include "wheelitem.hh"
#include "illegalmoveexception.hh"
#include "dialogask.hh"
#include "dialogwin.hh"


DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../../UI/ \
               ../../../GameLogic/Engine/

DEPENDPATH  += ../../../UI/ \
               ../../../GameLogic/Engine/
