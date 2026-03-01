#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static char *_compactador_de_arquivos(const char *arquivo) {
    uLong sourceLen = strlen(arquivo) + 1;
    uLong destLen = compressBound(sourceLen);

    unsigned char* compressedData = (unsigned char*)malloc(destLen);

    int res = compress(compressedData, &destLen, (const Bytef*)arquivo, sourceLen);

    if (res == Z_OK) 
        return compressedData;
    
    printf("Erro na compressão.\n");
    return "";
}

char *compactador_de_arquivos(const char *arquivo) {
    return _compactador_de_arquivos(arquivo);
}