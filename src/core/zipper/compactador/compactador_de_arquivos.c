#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>
#include "../../../../includes/types/zipper_file.h"

static ZipperFile _compactador_de_arquivos(const char *arquivo) {
    uLong sourceLen = strlen(arquivo) + 1;
    uLong destLen = compressBound(sourceLen);

    unsigned char* compressedData = (unsigned char*)malloc(destLen);

    int res = compress(compressedData, &destLen, (const Bytef*)arquivo, sourceLen);

    ZipperFile zipperFile;
    zipperFile.conteudoComprimido = NULL;
    zipperFile.tamanhoComprimido = 0;
    zipperFile.tamanhDescomprimido = 0;

    if (res == Z_OK) {
        zipperFile.conteudoComprimido = compressedData;
        zipperFile.tamanhoComprimido = destLen;
        zipperFile.tamanhDescomprimido = sourceLen;
        
        return zipperFile;
    }
    
    printf("Erro na compressão.\n");
    free(compressedData);
    return zipperFile;
}

ZipperFile compactador_de_arquivos(const char *arquivo) {
    return _compactador_de_arquivos(arquivo);
}