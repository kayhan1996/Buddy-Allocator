NAME = malloc
CC = gcc
CCFLAGS = -g

SRC_DIR = src
HEADER_DIR = include
BUILD_DIR = build
BIN_DIR = bin

all : build

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CCFLAGS) -I$(HEADER_DIR) -c $< -o $@

FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.c.o)
HEADER_FILES += $(wildcard $(HEADER_DIR))

build: $(OBJ_FILES) $(HEADER_FILES)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CCFLAGS) $(OBJ_FILES) -o $(BIN_DIR)/$(NAME).o

.PHONY : clean
clean :
	rm -rf build
	rm -rf $(BIN_DIR)

.PHONY : run
run : build
	@./$(BIN_DIR)/$(NAME).o

rebuild: clean build