#include <stdio.h>
#include <sys/stat.h>
#include "../../includes/core/criar_diretorio.h"
#include "../../includes/core/identifica_arquivos.h"

static int _cria_diretorios(char *dir) {

    printf(dir);

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

    char *dir[3] = {".vsr", ".vsr/objects", ".vsr/logs"};
    int err = 0;

    for(int i = 0; i < 3; i++) {
        err = _cria_diretorios(dir[i]);
        if(err) return 1;
    }

    char **filePaths = NULL;
    int totalArquivos = 0;
    
    identifica_arquivos(".", 0, &filePaths, &totalArquivos); 
    for (int i = 0; i < totalArquivos; i++) {
        printf("%s\n",filePaths[i]);
    }

    printf("Repositório iniciado com sucesso.\n");

    return 0;
}
