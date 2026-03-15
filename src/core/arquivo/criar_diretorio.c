#include <stdio.h>
#include <sys/stat.h>
#include "../../../includes/core/criar_diretorio.h"

static int _cria_diretorio(const char *nomeDiretorio) {
    if (mkdir(nomeDiretorio, 0777) == 0) {
        printf("Diretório criado: %s\n", nomeDiretorio);
        return 0;
    }

    printf("Diretório já existe ou erro ao criar: %s\n", nomeDiretorio);
    return 1;
}

int cria_diretorio(const char *nomeDiretorio) {
    _cria_diretorio(nomeDiretorio);
}