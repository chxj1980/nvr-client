CURDIR = $(shell pwd)
ROOTDIR = ../..
CURWORKDIR = $(CURDIR)/$(ROOTDIR)
include $(ROOTDIR)/Rules.make

SrcDir = src/
ObjDir = obj/
Src = $(wildcard $(SrcDir)*.c)
SrcFile = $(notdir $(Src))
ObjFile = $(patsubst %.c,%.o,$(SrcFile))
Obj = $(addprefix $(ObjDir),$(ObjFile))
CC := $(CROSS_PREFIX)gcc
AR := $(CROSS_PREFIX)ar

CFLAGS := $(CFLAGS_EXTRA) -I$(DECODER_DIR)/osa/inc -fPIC
LDFLAGS := $(LDFLAGS_EXTRA) -shared -fPIC


OUTPUT	:= libosa.so

all:main

main:$(Obj)
	$(CC) -o $(OUTPUT) $^ $(LDFLAGS)

.depend:$(Src)
	@$(CC) $(CFLAGS) -MM $(Src) > $@
sinclude .depend

$(ObjDir)%.o:$(SrcDir)%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean install

clean:
	-rm -f $(OUTPUT)
	rm -f $(ObjDir)*.o
	-rm -f .depend

install:
	install $(OUTPUT) $(CURDIR)/../player/lib
