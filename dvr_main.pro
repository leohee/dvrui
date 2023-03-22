# -------------------------------------------------
# Project created by QtCreator 2010-06-14T13:58:02
# -------------------------------------------------
QT += qt3support
TARGET = dvrmain
TEMPLATE = app
SOURCES += main.cpp \
    dvr_main.cpp \
    dvr_toolbar_main.cpp \
    dvr_statusbar.cpp \
    dvr_layout_main.cpp \
    dvr_util.cpp \
    dvr_timelinebar_play.cpp \
    dvr_setup.cpp \
    dvr_toolbar_sub.cpp \
    dvr_login.cpp \
    dvr_message.cpp \
    notifydialog.cpp    \
    WidgetKeyboard.cpp \
    dvr_formatconfirm.cpp \
    eventlogdialog.cpp \
    dvr_sidebar.cpp \
    dvr_setup_display.cpp \
    dvr_setup_camera.cpp \
    dvr_setup_record.cpp \
    dvr_setup_event.cpp \
    dvr_setup_dio.cpp \
    dvr_setup_audio.cpp \
    dvr_setup_storage.cpp \
    dvr_setup_backup.cpp \
    dvr_setup_network.cpp \
    dvr_setup_system.cpp
HEADERS += dvr_main.h \
	dvr_defines.h \
	dvr_app_api.h \
    struct.h \
    define.h \
    dvr_toolbar_main.h \
    coordinator.h \
    dvr_statusbar.h \
    dvr_layout_main.h \
    dvr_util.h \
    strdefine.h \
    dvr_timelinebar_play.h \
    dvr_setup.h \
    predefine.h \
    dvr_toolbar_sub.h \
    dvr_login.h \
    dvr_message.h \
    notifydialog.h  \
    WidgetKeyboard.h \
    dvr_formatconfirm.h \
    eventlogdialog.h \
    dvr_sidebar.h
FORMS += dvr_main.ui \
    dvr_toolbar_main.ui \
    dvr_statusbar.ui \
    dvr_layout_main.ui \
    dvr_timelinebar_play.ui \
    dvr_setup.ui \
    dvr_toolbar_sub.ui \
    dvr_login.ui \
    dvr_message.ui \
    notifydialog.ui \
    WidgetKeyboard.ui \
    dvr_formatconfirm.ui \
    eventlogdialog.ui \
    dvr_sidebar.ui
RESOURCES += dvr_main.qrc

INCLUDEPATH += ../app/inc
LIBS += -L../../../bin/ti816x/firmware -ldm816x
LIBS += -L$(linuxdevkit_PATH)/usr/lib -lasound
HEADERS += ../app/inc/app_manager.h
OBJECTS_DIR = ./debug/
