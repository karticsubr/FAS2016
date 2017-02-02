#
#  QCustomPlot Plot Examples
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = visualizer
TEMPLATE = app
INCLUDEPATH += /usr/local/Cellar/boost/1.60.0_2/include/
#DESTDIR=./build-visualization/ #Target file directory
#OBJECTS_DIR=./build-visualization/ #Intermediate object files directory
#MOC_DIR=./build-visualization/ #Intermediate moc files directory

SOURCES += main.cpp\
           mainwindow.cpp \
           qcustomplot.cpp \
           read-points.cpp \
           other-plots.cpp \
           core/create-folders.cpp

HEADERS  += mainwindow.h \
            qcustomplot.h \
            core/utils.h \
            core/io.h

FORMS    += mainwindow.ui

macx {
#QMAKE_CXXFLAGS += $$(CXXFLAGS)
#QMAKE_CFLAGS += $$(CFLAGS)
#QMAKE_LFLAGS += $$(LDFLAGS)

#     OBJCXXFLAGS="-stdlib=libc++ -std=c++11" LDFLAGS=-stdlib=libc++
#    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -stdlib=libc++ -std=c++11
#    CONFIG +=c++11
#    QMAKE_CXXFLAGS += -std=c++11
#    QMAKE_LFLAGS = -std=c++11
#    include(common.pri)
#    _BOOST_PATH = /usr/local/Cellar/boost/1.60.0_1
#    INCLUDEPATH += "$${_BOOST_PATH}/include/"
    LIBS += -L/usr/local/lib
    LIBS += -lboost_chrono -lboost_system -lboost_program_options -lboost_filesystem -lboost_date_time -lboost_iostreams # using dynamic lib (not sure if you need that "-mt" at the end or not)
    #LIBS += $${_BOOST_PATH}/lib/libboost_chrono-mt.a # using static lib
}

