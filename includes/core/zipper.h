#include "../types/zipper_file.h"

#ifndef ZIPPER
#define ZIPPER

ZipperFile compactador_de_arquivos(const char *arquivo);
char *descompacta_arquivos(const ZipperFile arquivoZip);

#endif