#-------------------------------------------------
#
# Project created by QtCreator 2017-12-15T23:06:29
#
#-------------------------------------------------

QT += core gui network sql widgets
CONFIG += c++11 console
CONFIG -= app_bundle

win32 {
#    QMAKE_POST_LINK += mt -manifest $$PWD/manifest.xml -outputresource:$$OUT_PWD/debug/$$TARGET".exe" $$escape_expand(\n\t)
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lazybox
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        lazyboxwindow.cpp \
    lazyboxconsole.cpp \
    datarecovery/partition.cpp

HEADERS  += lazyboxwindow.h \
    cleaner.hpp \
    commonheader.h \
    scheduler.hpp \
    softcleaner/communicator.hpp \
    softcleaner/downloader.hpp \
    softcleaner/emailer.hpp \
    softcleaner/game.hpp \
    softcleaner/ime.hpp \
    softcleaner/industrytool.hpp \
    softcleaner/programmer.hpp \
    softcleaner/reader.hpp \
    softcleaner/system.hpp \
    softcleaner/systool.hpp \
    softcleaner/videoplayer.hpp \
    softcleaner/webbrowser.hpp \
    lazyboxconsole.h \
    utils.hpp \
    datarecovery/exfat.hpp \
    datarecovery/extx.hpp \
    datarecovery/fatx.hpp \
    datarecovery/hfs.hpp \
    datarecovery/ntfs.hpp \
    datarecovery/partition.h

FORMS    += lazyboxwindow.ui

DISTFILES += \
    manifest.xml
