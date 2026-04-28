#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../../includes/core/criar_diretorio.h"

static int _cria_diretorios(char *dir) {
    int err = 0;
    err = cria_diretorio(dir);
    if(err) {
        printf("Erro ao criar o diretório '%s'", dir);
        return 1;
    }

    return err;
}

int command_start(void) {
    printf("Inicializando repositório VSR...\n");

    char *dir[5] = {".vsr", ".vsr/objects", ".vsr/logs", ".vsr/refs", ".vsr/refs/heads"};
    int err = 0;

    for(int i = 0; i < 5; i++) {
        err = _cria_diretorios(dir[i]);
        if(err) return 1;
    }

    printf("cria arquivos\n");

    // - Cria HEAD e aponta para a "main"
    FILE *head = fopen(".vrs/HEAD", "w");
    fprintf(head, "ref: refs/heads/main\n");
    fclose(head);

    // - Cria main
    FILE *main = fopen(".vsr/refs/heads/main", "w");
    fclose(main);

    // - Cria index
    FILE *index = fopen(".vrs/index", "w");
    fclose(index);

    printf("Repositório iniciado com sucesso.\n");

    return 0;
}
