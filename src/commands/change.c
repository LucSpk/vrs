#include <stdio.h>
#include <string.h>

#include "../../includes/core/io.h"

static int _command_change(char branchName[]) {
    // 1. Verifica se branch existe na ref
    char path[] = "./.vsr/refs/heads/";
    strcat(path, branchName);

    int err = 0;
    err = verifica(path);
    if(err) {
        // 2. Caso não exita lance um erro
        printf("Ramificação não existe");
        return 1;
    }

    // 3. Caso exista altere o conteudo do HEAD para apontar a ref
    FILE *headWriteFile = fopen("./.vsr/HEAD", "w");
    fprintf(headWriteFile, "ref: refs/heads/%s\n", branchName);

    fclose(headWriteFile);
    return 0;
}

int command_change(char branchName[]) {
    return _command_change(branchName);
}