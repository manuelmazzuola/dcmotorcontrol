# Copyright 1994-2009 The MathWorks, Inc.
#
# File    : ControlloTXRX.mk   $Revision: 1.26.4.47 $
#
# Abstract:
#       Real-Time Workshop template makefile for building a PC-based
#       stand-alone embedded real-time version of Simulink model using
#       generated C code and 
#			LCC compiler Version 2.4
#
#       This makefile attempts to conform to the guidelines specified in the
#       IEEE Std 1003.2-1992 (POSIX) standard. It is designed to be used
#       with GNU Make (gmake) which is located in matlabroot/bin/win32.
#
#       Note that this template is automatically customized by the Real-Time
#       Workshop build procedure to create "<model>.mk"
#
#       The following defines can be used to modify the behavior of the
#       build:
#         OPT_OPTS       - Optimization options. Default is none. To enable 
#                          debugging specify as OPT_OPTS=-g4. 
#         OPTS           - User specific compile options.
#         USER_SRCS      - Additional user sources, such as files needed by
#                          S-functions.
#         USER_INCLUDES  - Additional include paths 
#                          (i.e. USER_INCLUDES="-Iwhere-ever -Iwhere-ever2")
#                          (For Lcc, have a '/'as file seperator before the 
#                          file name instead of a '\' . 
#                          i.e.,  d:\work\proj1/myfile.c - reqd for 'gmake')
#       This template makefile is designed to be used with a system target
#       file that contains 'rtwgensettings.BuildDirSuffix' see ert.tlc


#------------------------ Macros read by make_rtw ------------------------------
#
# The following macros are read by the Real-Time Workshop build procedure:
#
#  MAKECMD         - This is the command used to invoke the make utility
#  HOST            - What platform this template makefile is targeted for
#                    (i.e. PC or UNIX)
#  BUILD           - Invoke make from the Real-Time Workshop build procedure
#                    (yes/no)?
#  SYS_TARGET_FILE - Name of system target file.

MAKECMD         = make
SHELL           = cmd
HOST            = PC
BUILD           = yes
SYS_TARGET_FILE = any
COMPILER_TOOL_CHAIN = avrgcc

MAKEFILE_FILESEP = /

#---------------------- Tokens expanded by make_rtw ----------------------------
#
# The following tokens, when wrapped with "|>" and "<|" are expanded by the
# Real-Time Workshop build procedure.
#
#  MODEL_NAME          - Name of the Simulink block diagram
#  MODEL_MODULES       - Any additional generated source modules
#  MAKEFILE_NAME       - Name of makefile created from template makefile <model>.mk
#  MATLAB_ROOT         - Path to where MATLAB is installed.
#  MATLAB_BIN          - Path to MATLAB executable.
#  S_FUNCTIONS         - List of additional S-function modules.
#  S_FUNCTIONS_LIB     - List of S-functions libraries to link. 
#  NUMST               - Number of sample times
#  NCSTATES            - Number of continuous states
#  BUILDARGS           - Options passed in at the command line.
#  MULTITASKING        - yes (1) or no (0): Is solver mode multitasking
#  INTEGER_CODE        - yes (1) or no (0): Is generated code purely integer
#  MAT_FILE            - yes (1) or no (0): Should mat file logging be done,
#                        if 0, the generated code runs indefinitely
#  EXT_MODE            - yes (1) or no (0): Build for external mode
#  TMW_EXTMODE_TESTING - yes (1) or no (0): Build ext_test.c for external mode
#                        testing.
#  EXTMODE_TRANSPORT   - Index of transport mechanism (e.g. tcpip, serial) for extmode
#  EXTMODE_STATIC      - yes (1) or no (0): Use static instead of dynamic mem alloc.
#  EXTMODE_STATIC_SIZE - Size of static memory allocation buffer.
#  MULTI_INSTANCE_CODE - Is the generated code multi instantiable (1/0)?
#  PORTABLE_WORDSIZES  - Is this build intented for SIL simulation with portable word sizes (1/0)?
#  SHRLIBTARGET        - Is this build intended for generation of a shared library instead 
#                        of executable (1/0)?
#  MAKEFILEBUILDER_TGT - Is this build performed by the MakefileBuilder class
#                        e.g. to create a PIL executable?
#  STANDALONE_SUPPRESS_EXE - Build the standalone target but only create object code modules 
#                            and do not build an executable

MODEL                = ControlloTXRX
MODULES              = ControlloTXRX_data.c 
MAKEFILE             = ControlloTXRX.mk
MATLAB_ROOT          = C:/MATLAB/R2010a
ALT_MATLAB_ROOT      = C:/MATLAB/R2010a
MATLAB_BIN           = C:/MATLAB/R2010a/bin
ALT_MATLAB_BIN       = C:/MATLAB/R2010a/bin
MASTER_ANCHOR_DIR    = 
START_DIR            = C:/Users/AMMINI~1/DOCUME~1/PROGET~1
S_FUNCTIONS          = 
S_FUNCTIONS_LIB      = 
NUMST                = 1
NCSTATES             = 0
BUILDARGS            =  GENERATE_REPORT=0 GENERATE_ASAP2=0
MULTITASKING         = 0
INTEGER_CODE         = 0
MAT_FILE             = 0
ONESTEPFCN           = 1
TERMFCN              = 1
B_ERTSFCN            = 0
MEXEXT               = mexw32
EXT_MODE             = 0
TMW_EXTMODE_TESTING  = 0
EXTMODE_TRANSPORT    = 0
EXTMODE_STATIC       = 0
EXTMODE_STATIC_SIZE  = 1000000
MULTI_INSTANCE_CODE  = 0
MODELREFS            = 
SHARED_SRC           = 
SHARED_SRC_DIR       = 
SHARED_BIN_DIR       = 
SHARED_LIB           = 
MEX_OPT_FILE         = -f 
PORTABLE_WORDSIZES   = 0
SHRLIBTARGET         = 0
MAKEFILEBUILDER_TGT  = 0
STANDALONE_SUPPRESS_EXE = 0
OPTIMIZATION_FLAGS      = atmega169
ADDITIONAL_LDFLAGS      = 

#--------------------------- Model and reference models -----------------------
MODELLIB                  = ControlloTXRXlib.lib
MODELREF_LINK_LIBS        = 
MODELREF_LINK_RSPFILE     = ControlloTXRX_ref.rsp
MODELREF_INC_PATH         = 
RELATIVE_PATH_TO_ANCHOR   = ..
# NONE: standalone, SIM: modelref sim, RTW: modelref rtw
MODELREF_TARGET_TYPE       = NONE
MODELREF_SFCN_SUFFIX       = _msf

#-- In the case when directory name contains space ---
ifneq ($(MATLAB_ROOT),$(ALT_MATLAB_ROOT))
MATLAB_ROOT := $(ALT_MATLAB_ROOT)
endif
ifneq ($(MATLAB_BIN),$(ALT_MATLAB_BIN))
MATLAB_BIN := $(ALT_MATLAB_BIN)
endif


#--------------------------- Tool Specifications -------------------------------
AVRHOME = C:/Programmi/WinAVR-20100110
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY)  \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000 

SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
COPY = cp
WINSHELL = cmd
LD = avr-gcc
MCU = $(OPTIMIZATION_FLAGS)
F_CPU = 8000000
FORMAT = ihex

AVRGCC =  avr-gcc
OPT = s
CSTANDARD = -std=gnu99
CDEFS = -DF_CPU=$(F_CPU)UL -mmcu=$(MCU)


MATH_LIB = -lm
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
PERL = $(MATLAB_ROOT)\sys\perl\win32\bin\perl

# Determine if we are generating an s-function
SFCN = 0
ifeq ($(MODELREF_TARGET_TYPE),SIM)
  SFCN = 1
endif
ifeq ($(B_ERTSFCN),1)
  SFCN = 1
endif

#------------------------------ Include Path -----------------------------------

# Additional includes 

ADD_INCLUDES = \
	-I$(START_DIR)/ControlloTXRX_ert_rtw \
	-I$(START_DIR) \


# see MATLAB_INCLUDES and COMPILER_INCLUDES from lcctool.mak

SHARED_INCLUDES =
ifneq ($(SHARED_SRC_DIR),)
SHARED_INCLUDES = -I$(SHARED_SRC_DIR) 
endif

AVR_INCLUDES= -I"$(MATLAB_ROOT)\rtw\c\src" -I"$(MATLAB_ROOT)\rtw\c\libsrc" \
              -I"$(MATLAB_ROOT)\rtw\c\src\ext_mode\common" \
              -I"$(MATLAB_ROOT)\extern\include" -I"$(MATLAB_ROOT)\simulink\include" \
              -I"C:\WinAVR\include"

INCLUDES = -I. -I$(RELATIVE_PATH_TO_ANCHOR) $(MATLAB_INCLUDES) $(ADD_INCLUDES) \
           $(COMPILER_INCLUDES) $(USER_INCLUDES) $(MODELREF_INC_PATH) \
           $(SHARED_INCLUDES) $(AVR_INCLUDES)

#-------------------------------- C Flags --------------------------------------

# Optimization Options
OPT_OPTS = $(CFLAGS)

# General User Options
OPTS =

# Compiler options, etc:
ifneq ($(OPTIMIZATION_FLAGS),)
CC_OPTS = $(OPTS) $(ANSI_OPTS) $(EXT_CC_OPTS) $(OPTIMIZATION_FLAGS)
MEX_OPT_OPTS = OPTIMFLAGS="$(OPTIMIZATION_FLAGS)"
else
CC_OPTS = $(OPT_OPTS) $(OPTS) $(ANSI_OPTS) $(EXT_CC_OPTS)
MEX_OPT_OPTS = $(OPT_OPTS)
endif

CPP_REQ_DEFINES = -DMODEL=$(MODEL) -DNUMST=$(NUMST) -DNCSTATES=$(NCSTATES) \
		  -DMAT_FILE=$(MAT_FILE) -DINTEGER_CODE=$(INTEGER_CODE) \
		  -DONESTEPFCN=$(ONESTEPFCN) -DTERMFCN=$(TERMFCN) \
		  -DHAVESTDIO -DMULTI_INSTANCE_CODE=$(MULTI_INSTANCE_CODE)

ifeq ($(MODELREF_TARGET_TYPE),SIM)
CPP_REQ_DEFINES += -DMDL_REF_SIM_TGT=1
else
CPP_REQ_DEFINES += -DMT=$(MULTITASKING)
endif

ifeq ($(PORTABLE_WORDSIZES),1)
CPP_REQ_DEFINES += -DPORTABLE_WORDSIZES
endif



CFLAGS = 
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O3
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD) $(INCLUDES)


#CFLAGS = $(CC_OPTS) $(CPP_REQ_DEFINES) $(INCLUDES) -w -noregistrylookup 


ifneq ($(ADDITIONAL_LDFLAGS),)
MEX_LDFLAGS = LINKFLAGS="$$LINKFLAGS $(ADDITIONAL_LDFLAGS)"
else
MEX_LDFLAGS =
endif

#-------------------------- Additional Libraries ------------------------------

LIBS =


LIBS += $(EXT_LIB) $(S_FUNCTIONS_LIB)

ifeq ($(MODELREF_TARGET_TYPE),SIM)
LIBMWIPP = $(MATLAB_ROOT)\lib\win32\libippmwipt.lib
LIBS += $(LIBMWIPP)
endif

ifeq ($(SFCN),1)
LIBFIXPT = $(MATLAB_ROOT)\extern\lib\win32\lcc\libfixedpoint.lib
LIBS += $(LIBFIXPT)
endif

ifeq ($(MODELREF_TARGET_TYPE),SIM)
LIBMWMATHUTIL = $(MATLAB_ROOT)\extern\lib\win32\lcc\libmwmathutil.lib
LIBS += $(LIBMWMATHUTIL)
endif

ifeq ($(MODELREF_TARGET_TYPE),SIM)
LIBMWSL_FILEIO = $(MATLAB_ROOT)\extern\lib\win32\lcc\libmwsl_fileio.lib
LIBS += $(LIBMWSL_FILEIO)
endif


#----------------------------- Source Files ------------------------------------
SRCS               += $(MODULES) $(MODEL).c $(EXT_SRC)


PRODUCT            = $(MODEL).hex
BIN_SETTING        = $(LD) $(LDFLAGS) $(ADDITIONAL_LDFLAGS) -o $(PRODUCT) $(SYSLIBS) 
BUILD_PRODUCT_TYPE = executable
PREBUILT_OBJS       = $(MODULES:.c=.o)


USER_SRCS = ert_main.c

USER_OBJS       = $(USER_SRCS:.c=.o)
LOCAL_USER_OBJS = $(notdir $(USER_OBJS))

OBJS      = $(SRCS:.c=.o) $(USER_OBJS)
LINK_OBJS = $(SRCS:.c=.o) $(LOCAL_USER_OBJS)

SHARED_OBJS := $(addsuffix .o, $(basename $(wildcard $(SHARED_SRC))))
FMT_SHARED_OBJS = $(subst /,\,$(SHARED_OBJS))

#--------------------------------- Rules ---------------------------------------



$(PRODUCT): $(MODEL).elf
		@echo sto convertendo in formato hex
		$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@
		@echo Creo un COFF utile per AVRstudio
		$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

		@echo "### Created" Butterfly program $(MODEL).hex

$(MODEL).elf : $(OBJS) $(SHARED_LIB) $(LIBS)
#	$(LD) -Os $(LDFLAGS) -o$(MODEL).elf $(OBJS)  -lm
	$(LD) -Os -o$(MODEL).elf $(LINK_OBJS) @$(MODELREF_LINK_RSPFILE) $(SHARED_LIB) $(LIBS)  $(LDFLAGS)

%.o : %.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : %.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/ert/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/ert/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/common/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/rtiostream/rtiostreamtcpip/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/rtiostream/rtiostreamtcpip/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/serial/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/serial/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/custom/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/custom/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<



%.o : $(MATLAB_ROOT)/rtw/c/src/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<



%.o : $(MATLAB_ROOT)/simulink/src/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.o : $(MATLAB_ROOT)/simulink/src/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

# Libraries:





#----------------------------- Dependencies ------------------------------------

$(OBJS) : $(MAKEFILE) rtw_proj.tmw

$(SHARED_OBJS) : $(SHARED_BIN_DIR)/%.o : $(SHARED_SRC_DIR)/%.c  
	$(CC) -c -Fo$@ $(CFLAGS) $<

$(SHARED_LIB) : $(SHARED_OBJS)
	@cmd /C "echo ### Creating $@"
	@if exist $@ del $@
	$(LIBCMD) /out:$@ $(FMT_SHARED_OBJS)
	@cmd /C "echo ### $@ Created"

#--------- Miscellaneous rules to purge, clean and lint (sol2 only) ------------

purge : clean
	@cmd /C "echo ### Deleting the generated source code for $(MODEL)"
	@del $(MODEL).c $(MODEL).h $(MODEL)_types.h $(MODEL)_data.c $(MODEL).rtw \
	        $(MODEL)_private.h $(MODULES) rtw_proj.tmw $(MAKEFILE)

clean :
	@cmd /C "echo ### Deleting the objects and $(PROGRAM)"
	@del $(LINK_OBJS) ..\$(MODEL).hex

# EOF: ert_lcc.tmf
