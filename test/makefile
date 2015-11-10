APPNAME=test
CPP=gcc
CFLAGS=
CPPFLAGS=
LDFLAGS=
DEFINEDS=

DEFINEDS+=-DUSE_SECURE_LIB

RM=rm -rf
PREFIX=@

INCLUDES=-I../safelib/include

OBJS= \
main.o \
../safelib/src/safelib_string.o \
../safelib/src/safelib_env.o \

all:$(OBJS)
	$(PREFIX) echo [LD] $(APPNAME)
	$(PREFIX) $(CPP) *.o -o $(APPNAME) $(CFLAGS) $(LDFLAGS)
	$(PREFIX) $(RM) *.o	
	$(PREFIX) echo DONE
%.o:%.cpp
	$(PREFIX) echo [CPP] $<
	$(PREFIX) $(CPP) -c $< $(CPPFLAGS) $(DEFINEDS) $(INCLUDES)
%.o:%.c
	$(PREFIX) echo [CC] $<
	$(PREFIX) $(CC) -c $< $(CFLAGS) $(DEFINEDS) $(INCLUDES)
clean:
	$(PREFIX) echo [RM] $(OBJS) $(APPNAME)
	$(PREFIX) $(RM) *.o
	$(PREFIX) $(RM) $(APPNAME)