#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include "../zipperfile.h"

static char *_descompacta_arquivos(const ZipperFile arquivoZip) {
    
    uLongf destLen = (uLongf)arquivoZip.tamanhDescomprimido;
    unsigned char *uncompressFille = malloc(destLen);

    int res = uncompress(
        (Bytef *)uncompressFille,
        &destLen,
        (const Bytef *)arquivoZip.conteudoComprimido,
        (uLongf)arquivoZip.tamanhoComprimido
    );

    if(res == Z_OK) 
        return (char *)uncompressFille;
    
    free(uncompressFille);
    return "";
}

char *descompacta_arquivos(const ZipperFile arquivoZip) {
    return _descompacta_arquivos(arquivoZip);
}