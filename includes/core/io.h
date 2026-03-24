#include "../types/arquivo.h"

#ifndef IO
#define IO

int verifica(char *path);
Arquivo le_conteudo_arquivo(const char *filePath);
void salva_arquivo_no_diretorio(const char *caminho, const char *fileName, const unsigned char *zipFile);

#endif
