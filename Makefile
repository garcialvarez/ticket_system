CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = ticket_app

# project layout
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# grab all our source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/ticket/ticket.c \
       $(SRC_DIR)/utils/utils.c

# map .c files to .o files in the build dir
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# default build target
all: $(TARGET) assets_dir

# link everything together into our final binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# compile individual objects, creating dirs on the fly as needed
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(INC_DIR)/ticket -I$(INC_DIR)/utils -c $< -o $@

# make sure we have a place to save the generated tickets
assets_dir:
	@mkdir -p assets

# wipe out the build folder and binary
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# handy shortcut to build and run in one go
run: all
	./$(TARGET)

# these aren't real files, just make commands
.PHONY: all clean run assets_dir
