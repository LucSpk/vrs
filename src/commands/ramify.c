#include <stdio.h>
#include <string.h>

#include "../../includes/core/io.h"

int command_ramify_from(char branchName[], char parentHash[]) {
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

    FILE *newBranchFile = fopen(path, "w");
    if(newBranchFile == NULL) {
        printf("Erro: Não foi possível criar o arquivo: %s\n", path);
        return 1;
    }

    fprintf(newBranchFile, "%s", parentHash);
    fclose(newBranchFile);

    // 4. Muda o ref para qual o HEAD aponta
    FILE *headWriteFile = fopen("./.vsr/HEAD", "w");
    fprintf(headWriteFile, "ref: refs/heads/%s\n", branchName);

    fclose(headWriteFile);

    printf("Ramificação '%s' criada com sucesso!\n", branchName);

    return 0;
}

static int _command_ramify(char branchName[]) {
    // 3. Caso não exista crie a ref com o nome da nova branch e o commit anterior da branch atual
    FILE *headFile = fopen("./.vsr/HEAD", "r");
    if(headFile == NULL) {
        printf("Erro: Falha ao abrir arquivo HEAD\n");
        return 1;
    }

    char ref[128];
    fgets(ref, sizeof(ref), headFile);
    ref[strcspn(ref, "\n")] = '\0';

    char refPath[128];
    sscanf(ref, "%*s %s", refPath);
    fclose(headFile);

    char completeRefPath[] = "./.vsr/";
    strcat(completeRefPath, refPath);
    FILE *refFile = fopen(completeRefPath, "r");
    if(refFile == NULL) {
        printf("Erro: Falha ao abrir arquivo ref.\n");
        return 1;
    }

    char parentHash[128];
    fgets(parentHash, sizeof(parentHash), refFile);
    parentHash[strcspn(parentHash, "\n")] = '\0';
    fclose(refFile);

    return command_ramify_from(branchName, parentHash);
}

int command_ramify(char branchName[]) {
    return _command_ramify(branchName);
}
