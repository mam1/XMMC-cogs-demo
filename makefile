
#PROPLIB = common
CFLAGS = -Os
NM = propeller-elf-nm
MODEL = xmmc
BOARD = C3F

# objects for this program
NAME = main
OBJS = main.o cogA.cog cogB.cog cogC.cog

all: $(NAME).elf

include common.mk
