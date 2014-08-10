
# Propeller Demo include
PROPLIB = common

#CFLAGS = -Os -xmmc
CFLAGS = -Os
NM = propeller-elf-nm
MODEL = xmmc
BOARD = C3F

#
# objects for this program
#

NAME = main
OBJS = main.o schedule.o cogA.cog cogB.cog cogC.cog rtc.cog

all: $(NAME).elf

include common.mk

#
# do a partial link of all driver code
#
# Note that any sections called .cog* (or *.cog) are automatically
# set up as overlays in cog memory space by the linker.
# So to load a COG C program from an LMM program, we

cogA.o: rtm_rmon.c
	$(CC) -r -Os -mcog -o $@ $<

cogB.o: rtm_tmon.c
	$(CC) -r -Os -mcog -o $@ $<

cogC.o: rtm_tmon.c
	$(CC) -r -Os -mcog -o $@ $<
    
#
# We have to avoid conflicts between symbols in the main C program and
# symbols in the local cog C programs. We do this by using objcopy to
# turn all the global symbols in the cog .text segment into local symbols
# (that's what --localize-text does). At the same time we rename
# the .text section to something ending in .cog (the filename works!) so
# that the linker knows to treat it as an overlay.
#

%.cog: %.o
	$(OBJCOPY) --rename-section .text=$@ --localize-text $^ $@

