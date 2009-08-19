include (../plugins/plugins.pri)
include (../lib/lib.pri)
include (../ananas.pri)
load(qsa)
QT +=network qsa

TARGET	= ananas-srv
DESTDIR = ../../bin

SOURCES	+= main.cpp httpdaemon.cpp clientsocket.cpp
HEADERS	+= httpdaemon.h \
	    clientsocket.h
	

TRANSLATIONS = \
    ../../translations/ananas-srv-en.ts \
    ../../translations/ananas-srv-ru.ts 
	
TEMPLATE	=app
INCLUDEPATH	+= ../lib ../lib/.ui

LIBS += -L../../lib -lananas4 -L../../lib/designer -lananasplugin4 -lqt4-qdataschema 

