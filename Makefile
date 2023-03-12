CPPFLAGS = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -march=armv7e-m --target=arm-none-eabi  
CPPFLAGS += -Wall -Wextra -Werror -Wpedantic -Wconversion -nostdlib
CPPFLAGS += -g3 -fpic

CXXFLAGS = -std=c++20
CFLAGS = -std=c11

AFLAGS =

BUILD_DIR = build
TARGET = a.out
OUT_TARGET = $(BUILD_DIR)/$(TARGET)

.PHONY: all clean

all: $(OUT_TARGET)

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

$(OUT_TARGET): $(OBJECT_FILES)
	clang++ $(CPPFLAGS) -mthumb -T f411re.ld -fuse-ld=lld -o $@ $(OBJECT_FILES)

clean:
	rm -r $(BUILD_DIR)
