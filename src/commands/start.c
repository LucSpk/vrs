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
    char *dir[5] = {".vsr", ".vsr/objects", ".vsr/logs", ".vsr/refs", ".vsr/refs/heads"};
    int err = 0;

    for(int i = 0; i < 5; i++) {
        err = _cria_diretorios(dir[i]);
        if(err) return 1;
    }

    // - Cria main
    FILE *main = fopen("./.vsr/refs/heads/main", "w");
    fclose(main);

    // - Cria HEAD e aponta para a "main"
    FILE *head = fopen("./.vsr/HEAD", "w");
    fprintf(head, "ref: refs/heads/main\n");
    fclose(head);

    // - Cria index
    FILE *index = fopen("./.vsr/index", "w");
    fclose(index);

    return 0;
}
