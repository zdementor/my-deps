SHELL	= sh

MODS	= mini miniOGL\
	  ministub minitile miniload minicache\
	  minitree minibrick minilod\
	  minipoint minitext minisky\
	  pnmbase pnmsample\
	  database datacloud datacache\
	  datacalc dataparse

SRCS	= $(MODS:=.cpp)
OBJS	= $(MODS:=.o)

AR	= ar -crs

CP	= cp -rfp
LN	= ln -fs
RM	= rm -rf

ZIP	= zip -qr9

.SUFFIXES: .cpp

.cpp.o:
	$(COMPILER) $(OPTS) -c $*.cpp

trap:

lib:	$(OBJS)
	@-$(AR) libMini.a $(OBJS)

example: lib
	$(COMPILER) $(OPTS) -o example -I. example.cpp -L. -lMini $(LINK) -lGL -lGLU -lm

stubtest: lib
	$(COMPILER) $(OPTS) -o stubtest -I. stubtest.cpp -L. -lMini -lm

viewer:	lib
	$(COMPILER) $(OPTS) -o viewer -I. viewer.cpp -L. -lMini $(LINK) -lGL -lGLU -lpthread -lcurl -lsquish -lm

depend:
	@-$(MAKEDEPEND) $(SRCS) >Makefile.d 2>/dev/null

install:
	@-$(CP) libMini.a /usr/local/lib
	@-if [ ! -e /usr/local/include/mini ]; then mkdir /usr/local/include/mini; fi
	@-$(CP) minibase.h minitime.h miniOGL.h mini.h pnmbase.h pnmsample.h *.hpp /usr/local/include/mini
	@-(cd /usr/local/include; $(LN) mini/minibase.h mini/miniOGL.h mini/mini.h mini/ministub.hpp mini/minitile.hpp mini/miniload.hpp mini/minicache.hpp mini/database.hpp mini/datacloud.hpp .)

clean:
	@-$(RM) $(OBJS) ii_files

tidy:	clean
	@-$(RM) *~ Makefile.d Debug Release *.ncb *.opt *.plg
	@-$(RM) libMini.a libMini example stubtest viewer

zip:	tidy
	@-(cd ..; $(ZIP) mini/MINI.zip mini -x \*/.\?\?\* -x \*CVS/\*)

sinclude Makefile.d
