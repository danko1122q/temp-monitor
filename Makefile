# Makefile for Ultimate Hardware Temperature Monitor
# Version 0.0.1
# FIX: Adjusted object compilation rules to correctly reference SRC_DIR

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm
DEBUG_FLAGS = -g -DDEBUG -O0

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/tempmonitor
TARGET_DEBUG = $(BIN_DIR)/tempmonitor-debug

# Source files
SOURCES = main.c sensor.c display.c utils.c
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
OBJECTS_DEBUG = $(SOURCES:%.c=$(BUILD_DIR)/%-debug.o)

# Header files
HEADERS = sensor.h display.h utils.h

# Colors for output
COLOR_RESET = \033[0m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_CYAN = \033[36m
COLOR_RED = \033[31m

# Default target
all: directories $(TARGET)
	@echo "$(COLOR_GREEN)✓ Build completed successfully!$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)Run: $(TARGET)$(COLOR_RESET)"

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Compile object files untuk build normal (FIXED)
# Target: build/main.o bergantung pada src/main.c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "$(COLOR_YELLOW)Compiling $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@
	
# Link executable
$(TARGET): $(OBJECTS)
	@echo "$(COLOR_YELLOW)Linking $(TARGET)...$(COLOR_RESET)"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: directories $(TARGET_DEBUG)
	@echo "$(COLOR_GREEN)✓ Debug build completed!$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)Run with: gdb $(TARGET_DEBUG)$(COLOR_RESET)"

# Compile object files untuk build debug (FIXED)
# Target: build/main-debug.o bergantung pada src/main.c
$(BUILD_DIR)/%-debug.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "$(COLOR_YELLOW)Compiling $< (debug)...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_DEBUG): $(OBJECTS_DEBUG)
	@echo "$(COLOR_YELLOW)Linking $(TARGET_DEBUG)...$(COLOR_RESET)"
	@$(CC) $(OBJECTS_DEBUG) $(LDFLAGS) -o $(TARGET_DEBUG)

# Install target
install: all
	@echo "$(COLOR_CYAN)Installing to /usr/local/bin...$(COLOR_RESET)"
	@sudo cp $(TARGET) /usr/local/bin/tempmonitor
	@sudo chmod +x /usr/local/bin/tempmonitor
	@echo "$(COLOR_GREEN)✓ Installation completed!$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)You can now run: tempmonitor$(COLOR_RESET)"

# Uninstall target
uninstall:
	@echo "$(COLOR_YELLOW)Removing from /usr/local/bin...$(COLOR_RESET)"
	@sudo rm -f /usr/local/bin/tempmonitor
	@echo "$(COLOR_GREEN)✓ Uninstallation completed!$(COLOR_RESET)"

# Clean build files
clean:
	@echo "$(COLOR_YELLOW)Cleaning build files...$(COLOR_RESET)"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "$(COLOR_GREEN)✓ Clean completed!$(COLOR_RESET)"

# Run the program
run: all
	@echo "$(COLOR_CYAN)Running temperature monitor...$(COLOR_RESET)"
	@$(TARGET)

# Run with stats
run-stats: all
	@echo "$(COLOR_CYAN)Running with statistics...$(COLOR_RESET)"
	@$(TARGET) -s

# Check for required modules
check-modules:
	@echo "$(COLOR_CYAN)Checking for loaded kernel modules...$(COLOR_RESET)"
	@lsmod | grep -E "coretemp|k10temp|zenpower|nct6775|it87" || \
		echo "$(COLOR_RED)⚠ No temperature sensor modules loaded!$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_YELLOW)Available hwmon devices:$(COLOR_RESET)"
	@ls -la /sys/class/hwmon/ 2>/dev/null || \
		echo "$(COLOR_RED)⚠ Cannot access /sys/class/hwmon/$(COLOR_RESET)"

# Setup lm-sensors
setup-sensors:
	@echo "$(COLOR_CYAN)Setting up lm-sensors...$(COLOR_RESET)"
	@echo "$(COLOR_YELLOW)This will install lm-sensors and detect your sensors.$(COLOR_RESET)"
	@sudo apt update
	@sudo apt install -y lm-sensors
	@sudo sensors-detect --auto
	@sudo systemctl restart kmod
	@echo "$(COLOR_GREEN)✓ Setup completed! Try running 'sensors' command.$(COLOR_RESET)"

# Load common modules
load-modules:
	@echo "$(COLOR_CYAN)Loading temperature sensor modules...$(COLOR_RESET)"
	@sudo modprobe coretemp 2>/dev/null && \
		echo "$(COLOR_GREEN)✓ Loaded coretemp (Intel)$(COLOR_RESET)" || \
		echo "$(COLOR_YELLOW)⚠ coretemp not available$(COLOR_RESET)"
	@sudo modprobe k10temp 2>/dev/null && \
		echo "$(COLOR_GREEN)✓ Loaded k10temp (AMD)$(COLOR_RESET)" || \
		echo "$(COLOR_YELLOW)⚠ k10temp not available$(COLOR_RESET)"
	@sudo modprobe zenpower 2>/dev/null && \
		echo "$(COLOR_GREEN)✓ Loaded zenpower (AMD Ryzen)$(COLOR_RESET)" || \
		echo "$(COLOR_YELLOW)⚠ zenpower not available$(COLOR_RESET)"

# List all sensors
list-sensors: all
	@$(TARGET) --list

# Help target
help:
	@echo "$(COLOR_CYAN)═══════════════════════════════════════════════════════════$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)Ultimate Hardware Temperature Monitor - Build System$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)═══════════════════════════════════════════════════════════$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_YELLOW)Available targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)make$(COLOR_RESET)              - Build the program (default)"
	@echo "  $(COLOR_GREEN)make debug$(COLOR_RESET)        - Build with debug symbols"
	@echo "  $(COLOR_GREEN)make install$(COLOR_RESET)      - Install to /usr/local/bin"
	@echo "  $(COLOR_GREEN)make uninstall$(COLOR_RESET)    - Remove from /usr/local/bin"
	@echo "  $(COLOR_GREEN)make clean$(COLOR_RESET)        - Remove build files"
	@echo "  $(COLOR_GREEN)make run$(COLOR_RESET)          - Build and run the program"
	@echo "  $(COLOR_GREEN)make run-stats$(COLOR_RESET)    - Run with statistics enabled"
	@echo ""
	@echo "$(COLOR_YELLOW)Sensor management:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)make check-modules$(COLOR_RESET)  - Check loaded kernel modules"
	@echo "  $(COLOR_GREEN)make load-modules$(COLOR_RESET)   - Load temperature sensor modules"
	@echo "  $(COLOR_GREEN)make setup-sensors$(COLOR_RESET)  - Install and configure lm-sensors"
	@echo "  $(COLOR_GREEN)make list-sensors$(COLOR_RESET)   - List all detected sensors"
	@echo ""
	@echo "$(COLOR_YELLOW)Usage examples:$(COLOR_RESET)"
	@echo "  make && sudo make install"
	@echo "  make run-stats"
	@echo "  make setup-sensors && make run"
	@echo ""

# Version info
version:
	@echo "$(COLOR_CYAN)Ultimate Hardware Temperature Monitor$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)Version: 0.0.1$(COLOR_RESET)"
	@echo "$(COLOR_YELLOW)Build system: GNU Make$(COLOR_RESET)"

.PHONY: all debug clean install uninstall run run-stats directories \
        check-modules setup-sensors load-modules list-sensors help version
