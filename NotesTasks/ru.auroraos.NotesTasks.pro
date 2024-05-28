TARGET = ru.auroraos.NotesTasks

CONFIG += \
    auroraapp

PKGCONFIG += \

SOURCES += \
    src/dbmanager.cpp \
    src/importexport.cpp \
    src/listitemmodel.cpp \
    src/main.cpp \
    src/mediaitem.cpp \
    src/note.cpp

HEADERS += \
    src/dbmanager.h \
    src/listitemmodel.h \
    src/mediaitem.h \
    src/note.h

DISTFILES += \
    qml/pages/ExportFile.qml \
    qml/pages/NewNotePage.qml \
    rpm/ru.auroraos.NotesTasks.spec \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

QT += \
    multimedia \
    sql

TRANSLATIONS += \
    translations/ru.auroraos.NotesTasks.ts \
    translations/ru.auroraos.NotesTasks-ru.ts \
