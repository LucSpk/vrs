#include <stdio.h>
#include <sys/stat.h>
#include "start.h"

static int cria_diretorio(const char *nome)
{
    if (mkdir(nome, 0777) == 0) {
        printf("Diretório criado: %s\n", nome);
        return 0;
    }

    printf("Diretório já existe ou erro ao criar: %s\n", nome);
    return 1;
}

int command_start(void)
{
    printf("Inicializando repositório VSR...\n");

    cria_diretorio(".vsr");
    cria_diretorio(".vsr/objects");

    printf("Repositório iniciado com sucesso.\n");

    return 0;
}