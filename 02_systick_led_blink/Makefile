MCPU=cortex-m4
MFPU=fpv4-sp-d16
ARCH=armv7e-m
FLASH_ADDR=0x08000000

CPPFLAGS = -mcpu=$(MCPU) -mfpu=$(MFPU) -march=$(ARCH) --target=arm-none-eabi  
CPPFLAGS += -Wall -Wextra -Werror -Wpedantic -Wconversion -nostdlib
CPPFLAGS += -g3 -fpic

CXXFLAGS = -std=c++20
CFLAGS = -std=c11

AFLAGS =

BUILD_DIR = build
TARGET = a
ELF_TARGET = $(BUILD_DIR)/$(TARGET).elf
BIN_TARGET = $(BUILD_DIR)/$(TARGET).bin

.PHONY: all clean flash

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
	clang -c $(CPPFLAGS) $(AFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	clang -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	clang++ -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(ELF_TARGET): $(OBJECT_FILES)
	clang++ $(CPPFLAGS) -mthumb -T f411re.ld -fuse-ld=lld -o $@ $(OBJECT_FILES)

$(BIN_TARGET): $(ELF_TARGET)
	llvm-objcopy-13 -O binary $(ELF_TARGET) $(BIN_TARGET)

clean:
	rm -r $(BUILD_DIR)

flash: $(BIN_TARGET)
	st-flash write $(BIN_TARGET) $(FLASH_ADDR)
