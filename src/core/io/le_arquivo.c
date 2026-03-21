#include <stdio.h>
#include <stdlib.h>
#include "../../../includes/types/arquivo.h"

static Arquivo _le_conteudo_arquivo(const char *filePath) {
    Arquivo resultado;
    resultado.tamanho = 0;
    resultado.conteudo = NULL;

    FILE *ptr_arquivo = fopen(filePath, "r");
    if(ptr_arquivo == NULL) {
        printf("ERRO! O arquivo não foi aberto!\n");
        return resultado;
    }
    
    // Vai para o final do arquivo
    fseek(ptr_arquivo, 0, SEEK_END);

    // Obtém o tamanho
    long tamanho = ftell(ptr_arquivo);

    // Volta para o início
    rewind(ptr_arquivo);

    // Aloca memória (+1 para '\0')
    char *buffer = (char *)malloc(tamanho + 1);
    if (buffer == NULL) {
        fclose(ptr_arquivo);
        return resultado;
    } 
    
    // Lê todo o conteúdo
    size_t lido = fread(buffer, 1, tamanho, ptr_arquivo);
    buffer[lido] = '\0';  // Finaliza string

    fclose(ptr_arquivo);

    resultado.tamanho = lido;
    resultado.conteudo = buffer;

    return resultado;
}

Arquivo le_conteudo_arquivo(const char *filePath) {
    return _le_conteudo_arquivo(filePath);
}