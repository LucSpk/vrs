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
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/rastreador/identifica_arquivos_do_diretorio/identifica_arquivos.c \
      $(SRC_DIR)/rastreador/leitor_de_arquivos/le_arquivo.c \
      $(SRC_DIR)/hash/criador_de_hash/cria_hash_do_arquivo.c \
      $(SRC_DIR)/zipper/compactador/compactador_de_arquivos.c \
	  $(SRC_DIR)/arquivo/salva_arquivo.c

# Gera os nomes dos objetos automaticamente
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Nome do executável
TARGET = programa

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