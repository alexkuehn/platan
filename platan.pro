TEMPLATE = app
TARGET = platan
QT += core \
    gui \
    xml \
    network
HEADERS += output/OutputMethodQC.h \
    output/OutputMethodMM.h \
    output/OutputMethodDebug.h \
    output/OutputMethod.h \
    output/outputmessage.h \
    input/inputmessage.h \
    output/OutputManager.h \
    process/ProcessManager.h \
    input/InputManager.h \
    PlatanApp.h \
    setupmedia.h \
    platan.h
SOURCES += output/OutputMethodQC.cpp \
    output/OutputMethodMM.cpp \
    output/OutputMethodDebug.cpp \
    output/OutputMethod.cpp \
    output/OutputManager.cpp \
    process/ProcessManager.cpp \
    input/InputManager.cpp \
    PlatanApp.cpp \
    setupmedia.cpp \
    main.cpp \
    platan.cpp
FORMS += setupmedia.ui \
    platan.ui
RESOURCES += platan.qrc
INCPATH += 'c:/Programme/QUANCOM/Qlib32/Inc'
LIBS += -L'c:/Programme/QUANCOM/Qlib32/Lib' -lQlib32 -lwinmm
TRANSLATIONS = platan_de.ts
RC_FILE = platan.rc