#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include "../zipperfile.h"

static char *_descompacta_arquivos(const ZipperFile arquivoZip) {
    
    z_stream stream = {0};

    int err;
    err = inflateInit(&stream);
    if(err != Z_OK) {
        printf("Erro ao iniciar o inflate!");
        return NULL;
    }

    stream.next_in = (Bytef *)arquivoZip.conteudoComprimido;
    stream.avail_in = (uInt)arquivoZip.tamanhoComprimido;

    size_t buffer_size = 1024;
    unsigned char *buffer = malloc(buffer_size);
    if (!buffer) {
        inflateEnd(&stream);
        return NULL;
    }

    stream.next_out = buffer;
    stream.avail_out = (uInt)buffer_size;

    while(1) {
        err = inflate(&stream, Z_NO_FLUSH);
        if (err == Z_STREAM_END)        // - Todos os dados foram descomprimidos?
            break;

        if(err != Z_OK) {               // - Algo deu errado?
            free(buffer);
            inflateEnd(&stream);
            return NULL;
        }

        // - avail_out é o tamanho disponível no buffer, caso igual == 0 - o buffer encheu.
        if(stream.avail_out == 0) {
            buffer_size *= 2;
            unsigned char *temp = realloc(buffer, buffer_size);
            if (!temp) {
                free(buffer);
                inflateEnd(&stream);
                return NULL;
            }
            buffer = temp;
        }

        // - Passa o que ja foi descompactado para o "novo buffer"
        stream.next_out = buffer + stream.total_out;
        stream.avail_out = (uInt)(buffer_size - stream.total_out);
        
    }

    inflateEnd(&stream);
    
    // Adiciona null terminator para segurança
    if (stream.total_out + 1 <= buffer_size) {
        buffer[stream.total_out] = '\0';
    }
    
    return (char *)buffer;
}

char *descompacta_arquivos(const ZipperFile arquivoZip) {
    return _descompacta_arquivos(arquivoZip);
}