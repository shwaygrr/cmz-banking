QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets uitools

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# Explicitly add the include path
INCLUDEPATH += include

SOURCES += \
    src/crypto/aes.cpp \
    src/crypto/aes_tables.cpp \
    src/bankaccount.cpp \
    src/helpers.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/user.cpp \
    src/transaction.cpp
    src/hash.cpp


HEADERS += \
    include/crypto/aes.h \
    include/crytpo/bigint.h \
    include/mainwindow.h \
    include/user.h \
    include/bankaccount.h \
    include/transaction.h \
    include/hash.h \
    include/transaction.h \
    src/bankwidget.h


FORMS += \
    forms/mainwindow.ui \
    forms/createwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
