# Compiler and flags
CC = gcc
AR = ar
CFLAGS = -Iinclude -fPIC
LDFLAGS =

# Directories
SRC_DIR = src
TEST_DIR = test
INCLUDE_DIR = include
BUILD_DIR = build
DIST_DIR = dist
INSTALL_DIR = /usr/lib
INCLUDE_INSTALL_DIR = /usr/local/include/chess

# Target and version info
OUTPUT_NAME = libchess
SO_NAME = $(OUTPUT_NAME).so
A_NAME = $(OUTPUT_NAME).a

EXEC = chess
CHECK = $(BUILD_DIR)/bin/check

# Determine the build type
ifneq ($(type), RELEASE)
    CFLAGS += -Wall -DDEBUG -ggdb
else
    CFLAGS += -O3
endif

# Source and object files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c' ! -name 'main.c')
TEST_FILES := $(shell find $(TEST_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_FILES))

# Default target
.DEFAULT_GOAL := help

# Total source file count
TOTAL_FILES := $(words $(SRC_FILES))

# Counter to track progress
counter = 0

# Targets

.PHONY: all
all: check_tools $(BUILD_DIR) shared static check exec ## Build all libraries
	@echo "Build complete."

.PHONY: check_tools
check_tools: ## Check if necessary tools are available
	@command -v $(CC) >/dev/null 2>&1 || { echo >&2 "[ERROR] $(CC) is not installed."; exit 1; }
	@command -v $(AR) >/dev/null 2>&1 || { echo >&2 "[ERROR] $(AR) is not installed."; exit 1; }

$(BUILD_DIR): ## Create the build directory if it doesn't exist
	@echo "[INFO] Creating build directory"
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/test
	mkdir -p $(BUILD_DIR)/grid
	mkdir -p $(BUILD_DIR)/grid/move
	mkdir -p $(BUILD_DIR)/bitboard
	mkdir -p $(BUILD_DIR)/ui
	mkdir -p $(BUILD_DIR)/bin

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c ## Compile source files
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c ## Compile test files
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: exec
exec: $(BUILD_DIR) static ## Build executable using static library
	@echo "[INFO] Building executable: $(EXEC)"
	@$(CC) src/main.c -o $(EXEC) -L. -l:$(A_NAME) -Iinclude

.PHONY: check
check: $(BUILD_DIR) static ## Build the tests
	@echo "[INFO] Building test executable: $(CHECK)"
	@$(CC) $(TEST_FILES) -o $(CHECK) -L. -l:$(A_NAME) -Iinclude

.PHONY: test
test: ## Build and run the tests
	make clean
	make all
	clear
	./$(CHECK)

.PHONY: shared
shared: $(OBJ_FILES) ## Build shared library
	@echo "[INFO] Building shared library: $(SO_NAME)"
	@$(CC) -shared $(CFLAGS) -o $(SO_NAME) $(OBJ_FILES)

.PHONY: static
static: $(OBJ_FILES) ## Build static library
	@echo "[INFO] Building static library: $(A_NAME)"
	@$(AR) rcs $(A_NAME) $(OBJ_FILES)

.PHONY: clean
clean: ## Remove all build files and libraries
	@echo "[INFO] Cleaning up build directory and libraries."
	@rm -rf $(BUILD_DIR) $(SO_NAME) $(A_NAME) $(EXEC) $(CHECK)

.PHONY: install
install: all ## Install library and headers
	# Install shared library
	sudo mkdir -p $(INSTALL_DIR)
	sudo cp $(SO_NAME) $(INSTALL_DIR)
	sudo chmod 755 $(INSTALL_DIR)/$(SO_NAME)
	
	# Install static library
	sudo cp $(A_NAME) $(INSTALL_DIR)
	sudo chmod 644 $(INSTALL_DIR)/$(A_NAME)
	
	# Install header files
	sudo rm -rf $(INCLUDE_INSTALL_DIR)
	sudo mkdir -p $(INCLUDE_INSTALL_DIR)
	sudo cp -r $(INCLUDE_DIR)/* $(INCLUDE_INSTALL_DIR)
	sudo chmod 644 $(INCLUDE_INSTALL_DIR)/*.h

compile_commands.json:
	bear -- make all

.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
