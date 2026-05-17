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
    // 2. Pega o hash do ultimo commit da brancg destino
    // 3. Le o HEAD
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