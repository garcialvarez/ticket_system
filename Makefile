CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = ticket_app

# Directorios de interés
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Archivos fuente a compilar
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/ticket/ticket.c \
       $(SRC_DIR)/utils/utils.c

# Generar archivos objeto esperados a partir de los .c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Targets base
all: $(TARGET) assets_dir

# Compilar ejecutable final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar objeto intermedio .c -> .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(INC_DIR)/ticket -I$(INC_DIR)/utils -c $< -o $@

# Target para generar la carpeta de text-files (según lo dicta el requerimiento)
assets_dir:
	@mkdir -p assets

# Limpiar compilación (archivos y ejecutable) para hacer clean builds
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Ejecuta todo previo a la corrida si no se ha hecho para estar seguro y ejecuta el CLI
run: all
	./$(TARGET)

# Prevenir malfuncionamiento si algún fichero en la ruta coincide con un nombre de Target
.PHONY: all clean run assets_dir
