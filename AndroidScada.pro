#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T15:45:03
#
#-------------------------------------------------

QT       += core gui script network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AndroidScada
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ArchivePage.cpp \
    TutorPage.cpp \
    WarningPage.cpp \
    ResMan/ArchiveManager.cpp \
    ResMan/ResManager.cpp \
    ResMan/SimulateMan.cpp \
    UI/AxisActPage.cpp \
    UI/CheckPage.cpp \
    UI/ExtraDevPage.cpp \
    UI/KeyboardDialog.cpp \
    UI/KJLBaseCtrl.cpp \
    UI/KJLButton.cpp \
    UI/KJLComboBox.cpp \
    UI/KJLEditBox.cpp \
    UI/KJLGroupBox.cpp \
    UI/KJLProgress.cpp \
    UI/KJLStatic.cpp \
    UI/KJLStatus.cpp \
    UI/KJLSubWin.cpp \
    UI/LayerPage.cpp \
    UI/OtherPage.cpp \
    UI/PreservePage.cpp \
    UI/SignalPage.cpp \
    UI/TutorEditDlg.cpp \
    UI/WaitPage.cpp \
    UI/ZtoolPage.cpp \
    ResMan/Protocol.c \
    ResMan/Communication.cpp

HEADERS  += mainwindow.h \
    ArchivePage.h \
    TutorPage.h \
    WarningPage.h \
    ResMan/ArchiveManager.h \
    ResMan/ResManager.h \
    ResMan/SimulateMan.h \
    UI/AxisActPage.h \
    UI/Basectrl.h \
    UI/CheckPage.h \
    UI/ExtraDevPage.h \
    UI/KeyboardDialog.h \
    UI/KJLBaseCtrl.h \
    UI/KJLButton.h \
    UI/KJLComboBox.h \
    UI/KJLEditBox.h \
    UI/KJLGroupBox.h \
    UI/KJLProgress.h \
    UI/KJLStatic.h \
    UI/KJLStatus.h \
    UI/KJLSubWin.h \
    UI/LayerPage.h \
    UI/OtherPage.h \
    UI/PreservePage.h \
    UI/SignalPage.h \
    UI/TutorEditDlg.h \
    UI/WaitPage.h \
    UI/ZtoolPage.h \
    ResMan/Device.h \
    ResMan/Protocol.h \
    ResMan/Communication.h \
    ResMan/plc_instruction.h

FORMS    += mainwindow.ui \
    WarningPage.ui \
    ArchivePage.ui \
    TutorPage.ui \
    UI/AxisActPage.ui \
    UI/CheckPage.ui \
    UI/ExtraDevPage.ui \
    UI/keyboardDialog.ui \
    UI/LayerPage.ui \
    UI/OtherPage.ui \
    UI/PreservePage.ui \
    UI/SignalPage.ui \
    UI/TutorEditDlg.ui \
    UI/WaitPage.ui \
    UI/Ztool.ui

CONFIG += mobility
MOBILITY =

RESOURCES += \
    mainwindow.qrc

DISTFILES += \
    cn.ts \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android


