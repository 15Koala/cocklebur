include ~/make_template/def.mk
# ####################################
# user configuring variables
# ####################################
ROOT =..

####### names #######
#[req] the name of compiled library
LIBNAME = cocklebur
#[req] the source file with the main function
MAINSRC = cocklebur_main.cpp
#[opt] the source file used for python's invoking
# PYTHONSRC = 
#[opt] the head file for so library
# CPPSOHEADER =

#[req] the (test) program this make generate
TESTNAME= cocklebur
Exclude_Cpp_files =
####### lib and labels #######
#[req] PROJ_L = -pthread -L../lib/ -lclucene -L../../3rdParty/lib/ -lboost_python -lpython2.6 
#PROJ_L = -L../3rdParty/lib/ -llog4cplus -lcachetube -lthrift-0.8.0 -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system -L../DAT -lFilter -L../json -ljson
PROJ_L = -L/usr/local/lib -lthrift-0.9.0 -pthread
#[opt] for muti-modules in exe
PROJ_L_MEXE=$(PROJ_L)
#[opt] for compile cpp's linking libraries
#e.g. CPPPROJ_L = -pthread -L../lib/ -lclucene
# CPPPROJ_L = 
#[req] include and marco definition for this project
#e.g. PROJ_I = -I../include -I../../3rdParty/include/boost/ -I/usr/include/python2.6  
PROJ_I =  -I/usr/local/include 
#[opt] the special flags for project
#e.g. PROJ_F = -D_REENTRANT -DFLEX -D_GNU_SOURCE -DLINUX -fpic 
PROJ_F = -DHAVE_NETINET_IN_H #-v

####### paths #######
#there is no '/' at the end of direcctory
LIBBASE  = ./lib
DPATH = ./obj-g
#DPATH = 
OPATH = ./obj

PURIFY  = purify -cache_dir=cache -always_use_cache_dir\
	  -windows=no -log_file=purify.log
#ALLSRC = $(wildcard *.cpp gen-cpp/*.cpp datatree/*.cpp)
ALLSRC = $(wildcard *.cpp gen-cpp/*.cpp datatree/*.cpp)
ALLOBJS = $(ALLSRC:%.cpp=$(OBJPATH)/%.o)
OUTPUTOBJ = $(OUTPUTMODULESRC:%.cpp=$(OBJPATH)/%.o)
OUTPUTSONAME = $(SOLIBBASE)/lib$(OUTPUTMODULE).so
INPUTOBJ = $(INPUTMODULESRC:%.cpp=$(OBJPATH)/%.o)
INPUTSONAME = $(SOLIBBASE)/lib$(INPUTMODULE).so
SOURCES = $(ALLSRC:$(MAINSRC)=)
#main sources files for muti-modules exe
SOURCES_EXE= $(filter-out $(OUTPUTMODULESRC) $(INPUTMODULESRC) $(Exclude_Cpp_files),$(SOURCES)) #$(SOURCES:$(OUTPUTMODULESRC)=)
EXESOURCES = $(SOURCES:$(PYTHONSRC)=)
DEPS = $(ALLSRC:.cpp=.d)
OBJS = $(SOURCES:%.cpp=$(OBJPATH)/%.o)
#object files for muti-modules exe
OBJS_EXE=$(SOURCES_EXE:%.cpp=$(OBJPATH)/%.o)
EXEOBJS = $(EXESOURCES:%.cpp=$(OBJPATH)/%.o)


ifndef INSTALLDIR
    INSTALLDIR =./bin
endif

INSTALLDIR_HOME = cocklebur-1.0.0
#static lib path
SLIBBASE  = $(LIBBASE)
#so lib path
SOLIBBASE = $(LIBBASE)

debug:
	$(MAKE) exe DBG="-DDEBUG"	
	
exe: $(TARGET)

clean:
#	@echo $(ALLSRC)
	$(RM) $(OPATH) $(DPATH) $(DEPS)

cleanall: clean
	$(RM) $(TESTNAME) $(TESTNAME)-g  $(LIBBASE)

install: $(TARGET) 
	@if [ ! -d $(INSTALLDIR) ]; then /bin/mkdir -p $(INSTALLDIR); fi
	@if [ ! -d $(INSTALLDIR)/$(INSTALLDIR_HOME) ]; then /bin/mkdir -p $(INSTALLDIR)/$(INSTALLDIR_HOME); fi	
	@if [ ! -d $(INSTALLDIR)/$(INSTALLDIR_HOME)/lib ]; then /bin/mkdir -p $(INSTALLDIR)/$(INSTALLDIR_HOME)/lib; fi	
	#$(CP) $(ARNAME) $(SONAME) $(INSTALLDIR)/realtime_filtering/lib
	$(CP) $(TARGET) $(INSTALLDIR)/$(INSTALLDIR_HOME)
	@if [ ! -d $(INSTALLDIR)/$(INSTALLDIR_HOME)/conf ]; then /bin/mkdir -p $(INSTALLDIR)/$(INSTALLDIR_HOME)/conf; fi	
	$(CP) conf/*.* $(INSTALLDIR)/$(INSTALLDIR_HOME)/conf
	$(CP) -f lib/* $(INSTALLDIR)/$(INSTALLDIR_HOME)/lib/

cleanLib:
	$(RM) $(ARNAME) $(SONAME)
	
$(TARGET): $(OBJS_EXE) $(MAINSRC)
	$(LL) $(CPPFLAGS) $(OBJS_EXE) $(MAINSRC) -o $(TARGET) $(PROJ_L_MEXE)


$(OBJPATH)/%.o:%.cpp
	@$(MKDIR)
	$(CC) $(CPPFLAGS) -c -o $@ $<

$(ARNAME):$(OBJS)
	$(MKDIR)
	$(AR) $(ARNAME) $(OBJS)

$(CPPSONAME):$(EXEOBJS)
	$(MKDIR)
	$(LD) $(INCLUDE) $(SOLABEL) -o $(CPPSONAME) $(EXEOBJS) $(CPPPROJ_L)

dependent: $(DEPS)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*, \1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
	
.PHONY: debug, exe, clean, cleanall, dependent, cleanLib
