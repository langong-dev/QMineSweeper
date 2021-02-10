QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    block.cpp \
    inputdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    block.h \
    inputdialog.h \
    mainwindow.h

FORMS += \
    inputdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    QMineSweeper.qrc


win32 {
    RC_ICONS = image/langong.ico
    RC_FILE = QMineSweeper.rc
}
unix {
    ICON = image/langong.icns
}

CONFIG(debug,debug|release) {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/debug)
} else {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/release)
}
