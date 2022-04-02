##########################################
# Parameters to configure the tweaks
#########################################
THREADS						?= 4  # Num of threads dispatched by OMP
NUM_RUNS					?= 10 # Num of runs to compute average of proc
ENABLE_SERIAL_RUN ?= 1
ENABLE_PARAL_RUN	?= 1
ENABLE_PROFIL_RUN	?= 0
USE_DOCKER				?= 0

##########################################
CMSIS_DIR    = ../CMSIS_5
DSP_DIR			 = $(CMSIS_DIR)/CMSIS/DSP
NN_DIR    	 = $(CMSIS_DIR)/CMSIS/NN
CORE_DIR     = $(CMSIS_DIR)/CMSIS/Core/Include

APPINCLUDE:=-I$(APP_ROOT) \
			-I$(APP_ROOT)/Include \
			-I$(CORE_DIR) \
			-I$(DSP_DIR)/Include \
			-I$(NN_DIR)/Include \

VERBOSE=1
WORKING_FOLDER=$(shell pwd)
TOOL_CHAIN=gcc

APP_ROOT?=$(WORKING_FOLDER)

MULT?=
DEF?=

-include NN.makefile.include
_CC_ARM	=		arm-linux-gnueabi-gcc
#_CC_ARM	=		aarch64-linux-gnu-gcc
#GNG_CC	=	arm-linux-gnueabi-gcc
ifeq ($(USE_DOCKER),1)
GNG_CC	=		docker run --rm -v "$(abspath ../)":/src -w /src/ml-cnn_cifar aignacio/arm_gcc $(_CC_ARM)
else
GNG_CC	=		arm-linux-gnueabihf-gcc
endif
SRCINC?=
ASSDBG=
INCOBJ=$(patsubst %.c,%$(MULT).$(CROSS).o,$(SRCINC))
EXE=$(patsubst %.c,%.o,$(SRC))
EXEPP=$(patsubst %.cpp,%.o,$(SRCPP))
CFLAGS += -O3
CFLAGS += -w -g -fopenmp
CFLAGS +=	-DTHREADS=$(THREADS)
CFLAGS +=	-DNUM_RUNS=$(NUM_RUNS)
ifeq ($(ENABLE_SERIAL_RUN),1)
CFLAGS +=	-DENABLE_SERIAL_RUN=1
endif
ifeq ($(ENABLE_PARAL_RUN),1)
CFLAGS +=	-DENABLE_PARAL_RUN=1
endif
ifeq ($(ENABLE_PROFIL_RUN),1)
CFLAGS +=	-DENABLE_PROFIL_RUN=1
endif

SRC+= \
     $(wildcard *.c)

SRCPP+=$(wildcard *.cpp)

SRCOBJ := $(patsubst %.c,%.o,$(SRC))

all: all_$(TOOL_CHAIN)

run:
	docker run --rm --name arm_gcc_test -v $(abspath .):/src -w /src aignacio/arm_gcc ./app.exe

#docker run --rm --name arm_gcc_test -it -v $(pwd):/src -w /src aignacio/arm_gcc ./cifar_original/ml-cnn_cifar/app.exe

ifeq ($(TOOL_CHAIN),gcc)

all_gcc: clean exe

exe::
	# Building testcase
	$(V) $(GNG_CC) $(CFLAGS) \
		$(SRC) \
		$(APPINCLUDE) -o app.exe

endif

# ---------------------------------------------------------------------------------------

clean::
	- rm -f *.$(CROSS).elf *.$(CROSS).o $(NN_DIR)/Source/*/*.o $(COREA_DIR)/Source/*.o $(DEVICE_DIR)/Source/*.o app.exe
