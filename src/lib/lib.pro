TARGET = QtVoiceTextWebAPI
MODULE = voicetextwebapi
QT = core gui network multimedia

load(qt_module)

PUBLIC_HEADERS =
HEADERS = $$PUBLIC_HEADERS \
    qtvoicetextwebapi_global.h \
    qvoicetextwebapi.h
SOURCES = \
    qvoicetextwebapi.cpp

DEFINES += QTVOICETEXTWEBAPI_LIBRARY
