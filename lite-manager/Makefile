TARGET    := lm
BUILD_DIR := build


COMPILER = gcc
CONFIG_DEBUG = y
CONFIG_LOG_LEVEL = 0
CONFIG_MEM_POOL_SIZE = 20
CONFIG_MACRO_CACHE_SIZE = 100

# Variables provided for Makefile
C_SOURCE := lm_macro.c lm_mem.c lm_parser.c lm_string.c lm_array.c lm_log.c lm_gen.c lm_cmd.c main.c heap_tlsf.c

C_PATH := -I.

C_FLAG := -O2 -Wl,-Bstatic -ffunction-sections -fdata-sections -nostdlib -ffreestanding -Wunused-function -Wall -Wextra -Werror -std=c99


# toolchain
CC_PREFIX ?= 
CC = $(CC_PREFIX)gcc
AS = $(CC_PREFIX)gcc -x assembler-with-cpp
CP = $(CC_PREFIX)objcopy
SZ = $(CC_PREFIX)size
OD = $(CC_PREFIX)objdump
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


CFLAGS    := $(C_PATH) $(C_DEFINE) $(C_FLAG) $(CPP_FLAG)
LDFLAGS   := $(LD_FLAG) $(LIB_PATH)


.PHONY: all
all: $(BUILD_DIR)/$(TARGET).exe target


# list of c and c++ program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(C_SOURCE)))))
vpath %.c $(sort $(dir $(C_SOURCE)))
vpath %.cpp $(sort $(dir $(C_SOURCE)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCE:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCE)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "CC   $<"
	@$(CC) -c $(CFLAGS) -MMD -MP \
		-MF  $(BUILD_DIR)/$(notdir $(<:.c=.d)) \
		-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	@echo "CC   $<"
	@$(CC) -c $(CFLAGS) -MMD -MP \
		-MF  $(BUILD_DIR)/$(notdir $(<:.cpp=.d)) \
		-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	@echo "AS   $<"
	@$(AS) -c $(CFLAGS) -MMD -MP  \
		-MF $(BUILD_DIR)/$(notdir $(<:.S=.d)) $< -o $@


$(BUILD_DIR)/$(TARGET).exe: $(OBJECTS) Makefile
	@echo "LD   $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(OD) $(BUILD_DIR)/$(TARGET).exe -xS > $(BUILD_DIR)/$(TARGET).s $@
	@strip $@

target: $(BUILD_DIR)/$(TARGET).exe
	@cp $< .


$(BUILD_DIR):
	@mkdir $@


# Pseudo command
.PHONY: clean

clean:
	@rm -R $(BUILD_DIR) $(TARGET).exe