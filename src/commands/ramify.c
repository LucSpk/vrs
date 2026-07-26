#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include "../../includes/core/io.h"

static int _command_ramify_from(char branchName[], char parentHash[]) {
    // 1. Verifica se ramificação ja existe nos refs
    char path[PATH_MAX];
    if (snprintf(path, sizeof(path), "./.vsr/refs/heads/%s", branchName) >= (int)sizeof(path)) {
        printf("Erro: nome de branch muito longo.\n");
        return 1;
    }

    int err = 0;
    err = verifica(path);
    if(!err) {
        // 2. Caso exista dê um erro "Ramificação ja existe"
        printf("Ramificação ja existe.\n");
        return 1;
    }

    FILE *newBranchFile = fopen(path, "w");
    if(newBranchFile == NULL) {
        printf("Erro: Não foi possível criar o arquivo: %s.\n", path);
        return 1;
    }

    fprintf(newBranchFile, "%s", parentHash);
    fclose(newBranchFile);

    // 4. Muda o ref para qual o HEAD aponta
    FILE *headWriteFile = fopen("./.vsr/HEAD", "w");
    if(headWriteFile == NULL) {
        printf("Erro: Não foi possível atualizar o arquivo HEAD.\n");
        return 1;
    }

    fprintf(headWriteFile, "ref: refs/heads/%s\n", branchName);
    fclose(headWriteFile);

    printf("Ramificação '%s' criada com sucesso!\n", branchName);

    return 0;
}

static int _command_ramify(char branchName[]) {
    // 3. Caso não exista crie a ref com o nome da nova branch e o commit anterior da branch atual
    FILE *headFile = fopen("./.vsr/HEAD", "r");
    if(headFile == NULL) {
        printf("Erro: Falha ao abrir arquivo HEAD.\n");
        return 1;
    }

    char ref[256];
    fgets(ref, sizeof(ref), headFile);
    ref[strcspn(ref, "\n")] = '\0';

    char refPath[PATH_MAX];
    if (sscanf(ref, "%*s %255s", refPath) != 1) {
        printf("Erro: formato inválido no arquivo HEAD.\n");
        fclose(headFile);
        return 1;
    }
    fclose(headFile);

    char completeRefPath[PATH_MAX];
    if (snprintf(completeRefPath, sizeof(completeRefPath), "./.vsr/%s", refPath) >= (int)sizeof(completeRefPath)) {
        printf("Erro: caminho de ref muito longo.\n");
        return 1;
    }

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

int command_ramify_from(char branchName[], char reference[]) {
    char hash[65];
    if (resolver_referencia_para_hash(reference, hash, sizeof(hash)) != 0) {
        printf("Erro: referência inválida '%s'.\n", reference);
        return 1;
    }
    return _command_ramify_from(branchName, hash);
}

int command_ramify(char branchName[]) {
    return _command_ramify(branchName);
}
