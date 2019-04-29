NAME = GW302 McHale V6 G-Wiz Baler

# Target file name (without extension).
TARGET = GW302XXX

#CC = arm-elf-gcc
#LD = arm-elf-ld -v
#AR = arm-elf-ar
#AS = arm-elf-as
#CP = arm-elf-objcopy
#OD = arm-elf-objdump
#SZ = arm-elf-size
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld -v
AR = arm-none-eabi-ar
AS = arm-none-eabi-as
CP = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
SZ = arm-none-eabi-size
ST = splint

LDSCRIPT=SAM7A3_Remap_Flash.ld
DEBUG=-ggdb3
# O0 for no optimisation, Os for maximum (options are O1,O2,O3,Os)
# O1 - check loops, some may need a volatile declaration so they are not optimized out! like 'mcm->status' 
OPTIM=-O1
AFLAGS = -ahls -mapcs-32

#-Wall \
#-Wextra \
#-Wredundant-decls \

CFLAGS = \
-Wa,-adhlns=$(subst $(suffix $<),.lst,$<) \
-I. \
-I./include \
-D SAM7A3_GCC \
-D THUMB_INTERWORK \
-mcpu=arm7tdmi \
-mthumb-interwork \
-ffunction-sections \
-fdata-sections \
-finline-functions \
-fomit-frame-pointer \
-Wunreachable-code \
-Wunused-function \
-Wunused \
-Wcast-align \
-Wimplicit \
-Wpointer-arith \
-Wswitch \
-Wreturn-type \
-Wshadow \
-T$(LDSCRIPT) \
$(OPTIM) \
$(DEBUG)

THUMB_FLAGS = -mthumb

## for ld link - now using gcc link, so path need not be specified!
#LI1 = -L C:/GNUARM-471_EABI/arm-none-eabi/lib/thumb/interwork
#LI2 = -L C:/GNUARM-471_EABI/lib/gcc/arm-none-eabi/4.7.1/thumb/interwork

LI3 = -Map $(TARGET)map --cref -T$(LDSCRIPT) --gc-sections --print-gc-sections
LI4 = -lnosys -lm -lc -lgcc -lg

## for gcc link - no library files need specifying - but currently only uses '/interwork' libs not '/thumb/interwork'
## resulting in an extra 17000+ bytes of code! - now fixed by also passing THUMB_FLAGS to linker!
MATH_LIB = -lm
#LFLAGS = -nostartfiles $(MATH_LIB) -Wl,-o$(TARGET).out,-M,-Map=$(TARGET).map,--cref,--gc-sections,--print-gc-sections
# sections removed can be seen in .map file in 'Discarded input sections'
LFLAGS = -nostartfiles $(MATH_LIB) -Wl,-o$(TARGET).out,-M,-Map=$(TARGET).map,--cref,--gc-sections


REMOVE = -R .IRamNV
CPHEX = -O ihex
CPBIN = -O binary
ODFLAGS = -x --syms

# Source files that need to be Assembled
ASSEMBLY_SRC = \
Cstartup.s

# Source files that can be built to THUMB mode.
THUMB_SRC = \
main.c \
heap_2.c \
functions.c \
display.c \
screens.c \
tasks.c \
queue.c \
list.c \
port.c \
serial.c \
comtest.c \
CANtest.c \
CAN_mcm.c \
TWI_task.c \
Operate.c \
Power.c \
bm_sym.c \
bm_num.c \
bm_f1all.c \
bm_f1eur.c \
bm_f1cyr.c \
strings.c

# Source files that must be built to ARM mode.
ARM_SRC = \
Cstartup_SAM7.c \
syscalls.c \
portISR.c \
interrupts.c

# Define all object files.
ASSEMBLY_OBJ = $(ASSEMBLY_SRC:.s=.o)
ARM_OBJ = $(ARM_SRC:.c=.o)
THUMB_OBJ = $(THUMB_SRC:.c=.o)

all: test

clean:
	-rm -f *.o *.lst $(TARGET).out $(TARGET).bin $(TARGET).hex $(TARGET).map $(TARGET).dmp *.obj *.~* *.tds *.bak

test: $(TARGET).out
	@ echo "...copying"
	$(CP) $(REMOVE) $(CPBIN) $(TARGET).out $(TARGET).bin
	padbin $(TARGET).bin
#	$(CP) $(REMOVE) $(CPHEX) $(TARGET).out $(TARGET).hex
	$(OD) $(ODFLAGS) $(TARGET).out > $(TARGET).dmp
	$(SZ) -x -B $(TARGET).out	

$(TARGET).out: $(ASSEMBLY_OBJ) $(ARM_OBJ) $(THUMB_OBJ) $(LDSCRIPT)
	@ echo "..linking"
#	$(LD) -o $(TARGET).out $(ASSEMBLY_OBJ) $(ARM_OBJ) $(THUMB_OBJ) $(LI1) $(LI2) $(LI3) $(LI4) $(OPTIM)
#	$(ST) -h +I./include +I../GW302 $(ARM_SRC) $(THUMB_SRC)
	$(CC) $(ASSEMBLY_OBJ) $(ARM_OBJ) $(THUMB_OBJ) $(THUMB_FLAGS) $(CFLAGS) $(LFLAGS)
	
$(ASSEMBLY_OBJ) : %.o: %.s makefile
	@ echo ".assembling"
	$(AS) $(AFLAGS) -o $*.o $< > $*.lst

$(ARM_OBJ) : %.o: %.c $(LDSCRIPT) makefile FreeRTOSConfig.h Board.h
	@ echo ".compiling"
#	$(ST) -h -c +I./include +I../GW302 $<
	$(CC) -c $(CFLAGS) $< -o $@
	
$(THUMB_OBJ) : %.o: %.c $(LDSCRIPT) makefile FreeRTOSConfig.h Board.h
	@ echo ".compiling"
#	$(ST) -h -c +I./include +I../GW302 $<
	$(CC) -c $(THUMB_FLAGS) $(CFLAGS) $< -o $@
	
