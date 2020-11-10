TARGET	= kmorph
VERSION	= 0.1.5

KDEBIN	= $(KDEDIR)/bin
KDEAPP	= $(KDEDIR)/share/apps
KDEAPPL	= $(KDEDIR)/share/applnk
KDEDOC	= $(KDEDIR)/share/doc
KDEHTML	= $(KDEDOC)/HTML
KDEICNS	= $(KDEDIR)/share/icons
KDETBAR	= $(KDEDIR)/share/toolbar
KDELOC	= $(KDEDIR)/share/locale

LSM	= $(TARGET)-$(VERSION).lsm

DATE	= `date +%x`

CPP	=	g++
CXX	=	g++
CC	=	gcc
LD	=	ld
MOC	=	moc
MSGFMT	= 	msgfmt
MAKE	=	make

CPPFLAGS	= -DVERSION='"$(VERSION)"' 
CXXFLAGS	= $(CPPFLAGS)

QTINCL	= -I$(QTDIR)/include
KDEINCL	= -I$(KDEDIR)/include
XINCL	= -I/usr/X11/include -I/usr/X11R6/include
INCL	= -I/usr/include/glib
CPPINCL = $(QTINCL) $(KDEINCL) $(XINCL) $(INCL)
CXXINCL = $(CPPINCL)

KDELIBS	= -L$(KDEDIR)/lib -lkdecore -lkdeui
QTLIBS	= -lqt
XLIBS	= -L/usr/X11R6/lib -lXpm -lXext -lX11
LIBS	= $(KDELIBS) $(QTLIBS) $(XLIBS)


KOBJS =		morph.o \
		kalpha.o \
		kmorphwin.o \
		kmorph.o

MOBJS =		kalpha.moc \
		kmorphwin.moc \
		kmorph.moc

.SUFFIXES:	.cpp

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $(CPPINCL) $<

.cc.o:
	$(CPP) $(CPPFLAGS) -c $(CPPINCL) $<

%.moc:	%.h
	$(MOC) $< -o $@

%.cpp:	%.h

all:	msg $(TARGET) locale

msg:
	@echo " "
	@echo " "
	@echo " "
	@echo " "
	@echo " "
	@echo " "
	@echo " "
	@echo " "
	@echo " "
	@echo " "

$(TARGET):	$(KOBJS)
		$(CPP) $(CPPFLAGS) -o $(TARGET) $(KOBJS) KFileDialog.o $(KLIBS) $(LIBS) 
		@echo "Make ready!"

clean:
		rm -f $(KOBJS) $(MOBJS) kmorph *~ */*~

morph.o:	morph.cpp morph.h
kmorph.o:	kmorph.cpp kmorph.h kmorph.moc
kmorphwin.o:	kmorphwin.cpp kmorphwin.h kmorphwin.moc
kalpha.o:	kalpha.cpp kalpha.h kalpha.moc

install:	all locale-install
		@echo Installing kmorph
		@echo
		cp $(TARGET) $(KDEBIN)
		mkdir -p $(KDEAPPL)/Graphics
		cp $(TARGET).kdelnk $(KDEAPPL)/Graphics
		cp icons/*.xpm $(KDETBAR)
		mkdir -p $(KDEHTML)/kmorph
		cp doc/*.html $(KDEHTML)/kmorph
		@echo
		@echo done.

deinstall:	uninstall

uninstall:	locale-uninstall
		@echo Uninstalling kmorph
		@echo
		rm -f $(KDEBIN)/$(TARGET)
		rm -f $(KDEAPPL)/Graphics/$(TARGET).kdelnk
		rm -f -r $(KDEHTML)/kmorph
		@echo
		@echo done.


locale:
		set -e; $(MAKE) -C po

locale-install:	locale
		set -e; $(MAKE) -C po install

locale-uninstall:
		set -e; $(MAKE) -C po uninstall


tgz:	clean lsm
	@echo Making tgz-package
	@echo
	cp -f ../$(LSM) .
	tar --directory .. --create --gzip --verbose --file=../$(TARGET)-$(VERSION).tgz $(TARGET)-$(VERSION)
	@echo
	@echo done.
 
lsm:
		@echo Making LSM
		@echo
		@echo Begin > ../$(LSM)
		@echo "Title:            "$(TARGET) >> ../$(LSM)
		@echo "Version:          "$(VERSION) >> ../$(LSM)
		@echo "Entered-date:     "$(DATE) >> ../$(LSM)
		@echo "Description:      Morphprogram for the K Desktop Environment" >> ../$(LSM)
		@echo "Description:      The code in not complete and prealpha," >> ../$(LSM)
		@echo "Description:      but right to test it." >> ../$(LSM)
		@echo "Description:      Comments to: wollgam@cww.de" >> ../$(LSM)
		@echo "Keywords:         kde morph graphic X11 Qt" >> ../$(LSM)
		@echo "Author:           Tobias Wollgam <wollgam@cww.de>" >> ../$(LSM)
		@echo "Maintained-by:    Tobias Wollgam <wollgam@cww.de>" >> ../$(LSM)
		@echo "Primary-site:     ftp://ftp.kde.org/pub/kde/apps/graphic/" >> ../$(LSM)
		@echo "Alternate-site:   " >> ../$(LSM)
		@echo "Original-site:    ftp://ftp.kde.org/pub/kde/apps/graphic/" >> ../$(LSM)
		@echo "Platform:         unix (only tested on linux)" >> ../$(LSM)
		@echo "Copying-policy:   GPL" >> ../$(LSM)
		@echo End >> ../$(LSM)
		@echo
		@echo done.


