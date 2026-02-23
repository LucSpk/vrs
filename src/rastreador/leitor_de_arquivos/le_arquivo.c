#include <stdio.h>

static void _le_conteudo_arquivo(char *filePath) {
    FILE *ptr_arquivo;
    
    ptr_arquivo = fopen(filePath, "r");
    if(ptr_arquivo == NULL) {
        printf("ERRO! O arquivo não foi aberto!\n");
    }
    printf("Sucesso! O arquivo foi aberto!\n");

    fclose(ptr_arquivo);
}

void le_conteudo_arquivo(char *filePath) {
    _le_conteudo_arquivo(filePath);
}