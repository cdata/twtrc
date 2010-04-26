AUTOMAKE_OPTIONS = foreign

CFLAGS = --pedantic -Wall -O2 -I/usr/include/curl -I/usr/include/libxml2
CPPFLAGS = --pedantic -Wall -O2 -I/usr/include/curl -I/usr/include/libxml2
LDFLAGS = -L/usr/lib -lcurl -lxml2

bin_PROGRAMS = twtrc
twtrc_SOURCES = main.cpp twitter.h twitter.cpp
