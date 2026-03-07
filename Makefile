# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -c

# Bibliotecas necessárias
LIBS = -lcrypto -lz

# Diretórios
SRC_DIR = src
BUILD_DIR = builds

# Arquivos fonte
# SRC = $(SRC_DIR)/main.c \
#       $(SRC_DIR)/rastreador/identifica_arquivos_do_diretorio/identifica_arquivos.c \
#       $(SRC_DIR)/rastreador/leitor_de_arquivos/le_arquivo.c \
#       $(SRC_DIR)/hash/criador_de_hash/cria_hash_do_arquivo.c \
#       $(SRC_DIR)/zipper/compactador/compactador_de_arquivos.c \
# 	  $(SRC_DIR)/arquivo/salva_arquivo.c

# Descobre todos os .c recursivamente e gera objetos correspondentes em builds/
SRC := $(shell find $(SRC_DIR) -type f -name '*.c')
# Gera os nomes dos objetos automaticamente
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Nome do executável
TARGET = programa
# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -c

# Bibliotecas necessárias
LIBS = -lcrypto -lz

# Diretórios
SRC_DIR = src
BUILD_DIR = builds
BIN_DIR = bin

# Descobre todos os .c recursivamente
SRC := $(shell find $(SRC_DIR) -type f -name '*.c')

# Gera os nomes dos objetos automaticamente
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Nome do executável
TARGET = $(BIN_DIR)/vsr

# Regra principal
all: $(TARGET)

# Linka o executável
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compila cada .c em .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
# Regra principal
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Como compilar cada .c em .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)