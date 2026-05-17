#include <stdio.h>
#include <string.h>

#include "../../includes/core/io.h"

static int _command_join(char *destino) {
    // 1. Verifica se a branch destino existe
    char path[] = "./.vsr/refs/heads/";
    strcat(path, destino);

    int err = verifica(path);
    if(err) {
        printf("Erro: Branch destino não existe.\n");
        return 1;
    }
    printf("Erro: Branch destino existe.\n");

    // 2. Pega o hash do ultimo commit da branch destino
    FILE *branchDestino = fopen(path, "w");
    if(branchDestino == NULL) {
        printf("Erro: Não foi possível aobri o arquivo: %s\n", path);
        return 1;
    }

    char hashCommitBranchDestino[128];
    fgets(hashCommitBranchDestino, sizeof(hashCommitBranchDestino), branchDestino);
    hashCommitBranchDestino[strcspn(hashCommitBranchDestino, "\n")] = '\0';
    fclose(branchDestino);

    // 3. Le o HEAD
    FILE *headFile = fopen("./.vsr/HEAD", "r");
    if(headFile == NULL) {
        printf("Erro: Falha ao abrir arquivo HEAD\n");
        return 1;
    }

    // 4. Pega o hash do ultimo commit da HEAD
    // 5. Le os commits das branches
    // 6. Le as trees dos commits
    // 7. Compara os arquivos: novo, alterado e deletado
    //       arquivo novo	                    adiciona
    //       arquivo deletado                   apaga
    //       arquivo alterado só numa branch	aceita
    //       arquivo alterado nas duas	        conflito
    // 8. Criar novo commit merge
    //        |  tree <nova_tree>
    //        |  parent <commit_main>
    //        |  parent <commit_branch>
    //      merge commit possui 2 parents
    // 9. Atualizar main
    return 0;
}

int command_join(char *destino) {
    return _command_join(destino);
}