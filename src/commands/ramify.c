#include <stdio.h>
#include <string.h>

#include "../../includes/core/io.h"

static int _command_ramify(char branchName[]) {
    // 1. Verifica se ramificação ja existe nos refs
    
    char path[] = "./.vsr/refs/heads/";
    strcat(path, branchName);

    int err = 0;
    err = verifica(path);
    if(!err) {
        // 2. Caso exista dê um erro "Ramificação ja existe"
        printf("Ramificação ja existe");
        return 1;
    }

    printf("%s\n", path);
    FILE *newBranchFile = fopen(path, "w");
    if(newBranchFile == NULL) {
        printf("Erro: Não foi possível criar o arquivo: %s\n", path);
        return 1;
    }

    // 3. Caso não exista crie a ref com o nome da nova branch e o commit anterior da branch atual
    // 4. Muda o ref para qual o HEAD aponta

    return 0;
}

int command_ramify(char branchName[]) {
    return _command_ramify(branchName);
}