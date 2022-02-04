#CC			=		gcc
CC_ARM	=		arm-linux-gnueabi-gcc
CC			=		$(CC_ARM)
CFLAGS	:=	-Wall -lm -fopenmp
ODIR		:=	output
_OBJ		+=	$(patsubst src/%,%,$(basename $(wildcard src/*.c)))
OBJ			:=	$(patsubst %,$(ODIR)/%,$(_OBJ))

$(shell   mkdir -p $(ODIR))

.PHONY: all clean

all: $(OBJ)
	@echo $(OBJ)
	@echo "Build done!"

$(ODIR)/%:	src/%.c
	@echo "Building $@"
	$(CC) $< $(CFLAGS) -o $@

clean:
	@rm -rf $(ODIR)
