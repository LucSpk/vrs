#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static void _compactador_de_arquivos(const char *arquivo) {
    uLong sourceLen = strlen(arquivo) + 1;

    // - Preparar buffer de saída
    uLong destLen = compressBound(sourceLen);
    unsigned char* compressedData = (unsigned char*)malloc(destLen);

    // - Compactar
    int res = compress(compressedData, &destLen, (const Bytef*)arquivo, sourceLen);

    if (res == Z_OK) {
        printf("Original: %lu bytes\n", sourceLen);
        printf("Compactado: %lu bytes\n", destLen);
    } else {
        printf("Erro na compressão.\n");
    }

    free(compressedData);
    return;
}

void compactador_de_arquivos(const char *arquivo) {
    _compactador_de_arquivos(arquivo);
}