Author = jzh

######################################
# target
######################################
TARGET = target


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# source path

# firmware library path
PERIFLIB_PATH = 

# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
# C_SOURCES =  $(shell find ./ -name '*.c')
C_SOURCES =	FWLIB/peripheral_gpio.c \
			FWLIB/peripheral_sysctrl.c \
			FWLIB/peripheral_pinctrl.c \
			FWLIB/peripheral_uart.c \
			FWLIB/peripheral_dma.c \
			FWLIB/peripheral_pwm.c \
			FWLIB/peripheral_ssp.c \
			FWLIB/peripheral_i2c.c \
			FWLIB/peripheral_keyscan.c \
			FWLIB/peripheral_qdec.c \
			FWLIB/peripheral_asdm.c \
			FWLIB/peripheral_pte.c \
			FWLIB/peripheral_timer.c \
			src/main.c 				\
			src/uart.c				\
			src/debug_print.c				\
			src/SEGGER_RTT.c				\
			src/SEGGER_RTT_printf.c				\

# ASM sources
ASM_SOURCES =  \
ing92000.s 


######################################
# firmware library
######################################
PERIFLIB_SOURCES = 


#######################################
# binaries
#######################################
# BINPATH = 
PREFIX = arm-none-eabi-
# CC = $(BINPATH)/$(PREFIX)gcc
# AS = $(BINPATH)/$(PREFIX)gcc -x assembler-with-cpp
# CP = $(BINPATH)/$(PREFIX)objcopy
# AR = $(BINPATH)/$(PREFIX)ar
# SZ = $(BINPATH)/$(PREFIX)size
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# float-abi

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

#-Dxxxxx
# C defines
C_DEFS =  

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES = -I FWLIB    \
           -I FWLIB/ing920    \
           -I src    \
		   -I CMSIS/Core/Include \
		   -I src/audioadc	\
		   -I src/iic	\
		   -I src/keyscan_demo	\
		   -I src/PTE/PTE_LIB	\
		   -I src/PTE/timer_pte	\
		   -I src/pwm	\
		   -I src/qdec	\
		   -I src/spi/spi_flash	\
		   -I src/spi/spi_normal	\
		   -I src/timer	\
		   -I src/uart	\



# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ing92000.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of ASM program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
# list of objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

flash:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

erase:
	st-flash erase
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***