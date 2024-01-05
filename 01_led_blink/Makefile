# TOOLCHAIN
CC = clang-14
CXX = clang++-14
OBJCOPY = llvm-objcopy-14

# Target device
MCPU = cortex-m4
MFPU = fpv4-sp-d16
ARCH = armv7e-m
FLASH_ADDR = 0x08000000
LINKER_SCRIPT = f411re.ld 

# Target-specific flags
CPPFLAGS = -mcpu=$(MCPU) -mfpu=$(MFPU) -march=$(ARCH) --target=arm-none-eabi -nostdlib -fpic
# Code quality
CPPFLAGS += -Werror -Wall -Wextra -Wpedantic -Wconversion -Wformat=2
CPPFLAGS += -g3

# Language-specific flags
CFLAGS = -std=c11
CXXFLAGS = -std=c++20

LDFLAGS = -mthumb -T $(LINKER_SCRIPT) -fuse-ld=lld

AFLAGS =

BUILD_DIR = build
TARGET = a
ELF_TARGET = $(BUILD_DIR)/$(TARGET).elf
BIN_TARGET = $(BUILD_DIR)/$(TARGET).bin

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

$(ELF_TARGET): $(OBJECT_FILES)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $(OBJECT_FILES)

$(BIN_TARGET): $(ELF_TARGET)
	$(OBJCOPY) -O binary $(ELF_TARGET) $(BIN_TARGET)

clean:
	rm -rf $(BUILD_DIR)

sca:
	compiledb -n make
	run-clang-tidy

flash: $(BIN_TARGET)
	st-flash write $(BIN_TARGET) $(FLASH_ADDR)
