QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets uitools

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# Explicitly add the include path
INCLUDEPATH += include

SOURCES += \
    src/activity.cpp \
    src/crypto/digsig.cpp \
    src/crypto/rsa.cpp \
    src/system.cpp \
    src/widgets/activitywidget.cpp \
    src/widgets/bankwidget.cpp \
    src/crypto/aes.cpp \
    src/crypto/aes_tables.cpp \
    src/bankaccount.cpp \
    src/helpers.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/user.cpp \
    src/crypto/hash.cpp \
    src/transaction.cpp \
    src/api/db.cpp


HEADERS += \
    include/activity.h \
    include/api/db.h \
    include/crypto/aes.h \
    include/crypto/digsig.h \
    include/crypto/rsa.h \
    include/crytpo/bigint.h \
    include/mainwindow.h \
    include/system.h \
    include/user.h \
    include/bankaccount.h \
    include/transaction.h \
    include/hash.h \
    include/transaction.h \
    include/widgets/bankwidget.h \
    include/widgets/activitywidget.h

FORMS += \
    forms/mainwindow.ui \
    forms/createwindow.ui \
    forms/dashboardwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
