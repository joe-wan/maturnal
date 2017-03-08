###################################################
#
# Makefile for maturnal
#
###################################################

#
# Macros
#

64BIT_VERSION ?= yes
OPTIMIZE ?= yes
USEREADLINE ?= yes
USEBOOST ?= no
BOOST_LIBRARY_DIR="\"Enter_your_boost_library_path_here\""
BOOST_INCLUDE_DIR="\"Enter_your_boost_include_path_here\""
MOTHUR_FILES="\"Enter_your_default_path_here\""
RELEASE_DATE = "\"3/6/2017\""
VERSION = "\"1.39.4\""

ifeq  ($(strip $(64BIT_VERSION)),yes)
    CXXFLAGS += -DBIT_VERSION
endif

# Fastest
ifeq  ($(strip $(OPTIMIZE)),yes)
    CXXFLAGS += -O3
endif

CXXFLAGS += -DRELEASE_DATE=${RELEASE_DATE} -DVERSION=${VERSION} -std=c++0x

ifeq  ($(strip $(MOTHUR_FILES)),"\"Enter_your_default_path_here\"")
else
    CXXFLAGS += -DMOTHUR_FILES=${MOTHUR_FILES}
endif


# if you do not want to use the readline library, set this to no.
# make sure you have the library installed


ifeq  ($(strip $(USEREADLINE)),yes)
    CXXFLAGS += -DUSE_READLINE
    LIBS += -lreadline
endif


#User specified boost library
ifeq  ($(strip $(USEBOOST)),yes)

    LDFLAGS += -L ${BOOST_LIBRARY_DIR}

    LIBS += -lboost_iostreams -lz
    CXXFLAGS += -DUSE_BOOST -I ${BOOST_INCLUDE_DIR}
endif

#
# INCLUDE directories for maturnal
#
#
    VPATH=source/calculators:source/classifier:source/commands:source/datastructures
    skipUchime := source/uchime_src/
    subdirs :=  $(sort $(dir $(wildcard source/*/)))
    subDirIncludes = $(patsubst %, -I %, $(subdirs))
    subDirLinking =  $(patsubst %, -L%, $(subdirs))
    CXXFLAGS += -I. $(subDirIncludes)
    LDFLAGS += $(subDirLinking)


#
# Get the list of all .cpp files, rename to .o files
#
    OBJECTS=$(patsubst %.cpp,%.o,$(wildcard $(addsuffix *.cpp,$(subdirs))))
    OBJECTS+=$(patsubst %.c,%.o,$(wildcard $(addsuffix *.c,$(subdirs))))
    OBJECTS+=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
    OBJECTS+=$(patsubst %.c,%.o,$(wildcard *.c))

maturnal : $(OBJECTS)
	$(CXX) $(LDFLAGS) $(TARGET_ARCH) -o $@ $(OBJECTS) $(LIBS)

install : maturnal


%.o : %.c %.h
	$(COMPILE.c) $(OUTPUT_OPTION) $<
%.o : %.cpp %.h
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
%.o : %.cpp %.hpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<



clean :
	@rm -f $(OBJECTS)
