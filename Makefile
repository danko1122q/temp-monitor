# Makefile for Temperature Monitor
# Version 0.0.1

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=200809L -Wno-format-truncation -Wno-stringop-truncation
LDFLAGS = -lm
DEBUG_FLAGS = -g -DDEBUG -O0

BUILD_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/temp
TARGET_DEBUG = $(BIN_DIR)/temp-debug

SOURCES = main.c sensor.c display.c utils.c
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
OBJECTS_DEBUG = $(SOURCES:%.c=$(BUILD_DIR)/%-debug.o)

HEADERS = sensor.h display.h utils.h main.h

all: directories $(TARGET)
	@echo "[OK] Build done: $(TARGET)"

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	@echo "[LD] $(TARGET)"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: directories $(TARGET_DEBUG)
	@echo "[OK] Debug build: $(TARGET_DEBUG)"

$(BUILD_DIR)/%-debug.o: %.c $(HEADERS)
	@echo "[CC] $< (debug)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_DEBUG): $(OBJECTS_DEBUG)
	@echo "[LD] $(TARGET_DEBUG)"
	@$(CC) $(OBJECTS_DEBUG) $(LDFLAGS) -o $(TARGET_DEBUG)

install: all
	@echo "[INSTALL] /usr/local/bin/temp"
	@sudo cp $(TARGET) /usr/local/bin/temp
	@sudo chmod +x /usr/local/bin/temp

uninstall:
	@echo "[REMOVE] /usr/local/bin/temp"
	@sudo rm -f /usr/local/bin/temp

clean:
	@echo "[CLEAN]"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

run: all
	@$(TARGET)

run-stats: all
	@$(TARGET) -s

list: all
	@$(TARGET) --list

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all         Build (default)"
	@echo "  debug       Build with debug"
	@echo "  install     Install to /usr/local/bin"
	@echo "  uninstall   Remove from system"
	@echo "  clean       Clean build files"
	@echo "  run         Build and run"
	@echo "  run-stats   Run with stats"
	@echo "  list        List sensors"
	@echo ""

.PHONY: all debug clean install uninstall run run-stats directories list help
