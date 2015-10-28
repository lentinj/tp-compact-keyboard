# kbuild trick to avoid linker error. Can be omitted if a module is built.
obj- := dummy.o

all: tp-compact-usb-keyboard

# List of programs to build
hostprogs-y := tp-compact-usb-keyboard

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_hid-example.o += -I$(objtree)/usr/include
