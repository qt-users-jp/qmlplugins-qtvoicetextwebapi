TARGETPATH = QtVoiceTextWebAPI

QT = qml voicetextwebapi
LIBS += -L$$QT.voicetextwebapi.libs

SOURCES += main.cpp

load(qml_plugin)

OTHER_FILES = plugins.qmltypes qmldir
