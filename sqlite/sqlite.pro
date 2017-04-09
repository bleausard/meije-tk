####################################################################################################

# QtCreator: only build the project

# export ANDROID_NDK_ROOT=/opt/android-ndk-r10e
# /opt/Qt/5.9/android_armv7/bin/qmake -o Makefile ../sqlite/sqlite.pro -spec android-g++ CONFIG+=debug CONFIG+=qml_debug

TARGET = qsqlite-at
TEMPLATE = lib

####################################################################################################

CONFIG += plugin
CONFIG += c++11
CONFIG += debug

QT -= gui
QT += core-private sql-private

PLUGIN_CLASS_NAME = QSQLiteDriverPlugin
PLUGIN_TYPE = sqldrivers

HEADERS += $$PWD/qsql_sqlite_at_p.h
SOURCES += $$PWD/qsql_sqlite_at.cpp $$PWD/smain.cpp

linux:!android {
# Qt is compiled on Linux using system SQLite
# It crashes if we compile SQLite source in the plugin.
LIBS += -lsqlite3
}
android {
# SOURCES += $$PWD/sqlite3.c
LIBS += $$PWD/../android-cmake-build/third-parties/sqlite/libsqlite.so
}

# CONFIG(release, debug|release):DEFINES *= NDEBUG
DEFINES += SQLITE_OMIT_COMPLETE SQLITE_ENABLE_FTS3 SQLITE_ENABLE_FTS3_PARENTHESIS SQLITE_ENABLE_RTREE
linux:!android {
DEFINES += HAVE_POSIX_FALLOCATE=1
}
DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

OTHER_FILES += sqlite.json

####################################################################################################

# Android Deployement

# android-build/libs/armeabi-v7a/libplugins_sqldrivers_libqsqlite.so
# /home/opt/Qt/5.9/android_armv7/plugins/sqldrivers/libqsqlite.so

# see ANDROID_DEPLOYMENT_DEPENDENCIES