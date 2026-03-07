#ifndef LE_ARQUIVO_H
#define LE_ARQUIVO_H

typedef struct {
    long tamanho;
    char *conteudo;
} Arquivo;

Arquivo le_conteudo_arquivo(const char *filePath);

#endif