# TOOLCHAIN
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy

# Target device
MCPU = cortex-m4
MFPU = fpv4-sp-d16
ARCH = armv7e-m
FLASH_ADDR = 0x08000000
LINKER_SCRIPT = f411re.ld 

# Debug
GDB_PORT = 3333
SVD_FILE = "../STM32F411.svd"

# Compiler, linker flags
CPPFLAGS = -mcpu=$(MCPU) -mfpu=$(MFPU) -march=$(ARCH) -nostdlib

# Code quality
CPPFLAGS += -Werror -Wall -Wextra -Wpedantic -Wconversion -Wformat=2
CPPFLAGS += -g3

# Language-specific flags
CFLAGS = -std=c11
CXXFLAGS = -std=c++20 -fno-exceptions -fno-rtti -Wno-deprecated-volatile
AFLAGS =

# Linker-specific flags
LDFLAGS = -mthumb

# Optimization
CPPFLAGS += -Os -flto
LDFLAGS += -flto

BUILD_DIR = build
TARGET = a
ELF_TARGET = $(BUILD_DIR)/$(TARGET).elf
BIN_TARGET = $(BUILD_DIR)/$(TARGET).bin
MAP_TARGET = $(BUILD_DIR)/$(TARGET).map

.PHONY: all clean flash format sca

all: $(BIN_TARGET)

# Collect all sources
SRC_DIR = src
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
A_SOURCES = $(wildcard $(SRC_DIR)/*.s)

SOURCES = $(A_SOURCES) $(C_SOURCES) $(CPP_SOURCES)

# List of object files in the build folder
SOURCES_BASE = $(basename $(SOURCES))
OBJECT_FILES = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%, $(addsuffix  .o, $(SOURCES_BASE)))

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CC) -c $(CPPFLAGS) $(AFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(ELF_TARGET): $(OBJECT_FILES) $(LINKER_SCRIPT)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -T $(LINKER_SCRIPT) -o $@ -Wl,-Map=$(MAP_TARGET) $(OBJECT_FILES)

$(BIN_TARGET): $(ELF_TARGET)
	$(OBJCOPY) -O binary $(ELF_TARGET) $(BIN_TARGET)

clean:
	rm -rf $(BUILD_DIR)

sca:
	compiledb -n make
	run-clang-tidy

flash: $(BIN_TARGET)
	[ "$(shell st-info --descr)" = "STM32F411xC_xE" ] || (echo "Invalid device" && false)
	st-flash --opt --reset write $(BIN_TARGET) $(FLASH_ADDR)

openocd:
	openocd -f /usr/local/share/openocd/scripts/interface/stlink.cfg -f /usr/local/share/openocd/scripts/board/st_nucleo_f4.cfg

gdb:
	st-util -m -p $(GDB_PORT)

debug: $(ELF_TARGET)
	gdb-multiarch --eval-command="target extended-remote :$(GDB_PORT)" --eval-command="svd_load $(SVD_FILE)" $(ELF_TARGET)
