#ifndef ZIPPER_FILE_H
#define ZIPPER_FILE_H

typedef struct 
{
    unsigned char *conteudoComprimido;
    long tamanhoComprimido;
    long tamanhDescomprimido;
} ZipperFile;

#endif
