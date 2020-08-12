ARM_PREFIX	= arm-none-eabi
CC		= $(ARM_PREFIX)-gcc
LD		= $(ARM_PREFIX)-gcc

INCLUDE		= -ICMSIS/Include
INCLUDE		+= -ICMSIS/Device/ST/STM32F0xx/Include

CFLAGS		+= -mlittle-endian -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -g3
AFLAGS		+= $(CFLAGS)
LDFLAGS		+= -TSTM32F030xC_FLASH.ld -nostartfiles -nostdlib

BUILDDIR = build

CSRC = main.c
ASRC = startup_stm32f030xc.s
ELFFILE = main.elf

COBJS = $(patsubst %, $(BUILDDIR)/%,$(CSRC:.c=.o))
AOBJS = $(patsubst %, $(BUILDDIR)/%,$(ASRC:.s=.o))

all: setup build

setup:
	@mkdir -p $(BUILDDIR)

build: setup $(COBJS) $(AOBJS)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $(ELFFILE) $(COBJS) $(AOBJS)

$(COBJS): $(BUILDDIR)/%.o : %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

$(AOBJS): $(BUILDDIR)/%.o : %.s
	$(CC) $(INCLUDE) $(AFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

flash:
	# TODO
