#include "../types/arquivo.h"

#ifndef HASH
#define HASH

char * cria_hash(const char *conteudo);
char * cria_hash_de_arquivo(const Arquivo arq);

#endif