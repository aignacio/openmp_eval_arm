# File              : makefile
# License           : MIT license <Check LICENSE>
# Author            : Anderson Ignacio da Silva (aignacio) <anderson@aignacio.com>
# Date              : 05.02.2022
# Last Modified Date: 06.02.2022

#CC			=		gcc
#_CC_ARM	=		arm-linux-gnueabi-gcc
#_CC_ARM	=		arm-linux-gnueabi-gcc
_CC			=		aarch64-linux-gnu-gcc
CC			=		docker run --rm -v "$(abspath .)":/src -w /src arm_gcc_aignacio $(_CC)
CFLAGS	:=	-Wall -lm -fopenmp -DTHREADS=10
ODIR		:=	output
_OBJ		+=	$(patsubst src/%,%,$(basename $(wildcard src/*.c)))
OBJ			:=	$(patsubst %,$(ODIR)/%,$(_OBJ))

$(shell   mkdir -p $(ODIR))

.PHONY: all clean build

# To run the toolchain in interactive mode, run the following in the terminal
#	docker run --rm --name arm_gcc_test -it -v $(pwd):/src -w /src arm_gcc_aignacio bash
run:
	docker run --rm --name arm_gcc_test -it -v $(abspath .):/src -w /src arm_gcc_aignacio output/NeuralNet

build:
	docker build -t arm_gcc_aignacio:latest ..

all: $(OBJ)
	@echo $(OBJ)
	@echo "Build done!"

$(ODIR)/%:	src/%.c
	@echo "Building $@"
	$(CC) $< $(CFLAGS) -o $@

clean:
	@echo "Cleaning compiled binaries...$(ODIR)"
	@rm -rf $(ODIR)
