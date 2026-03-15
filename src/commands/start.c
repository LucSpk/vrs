#include <stdio.h>
#include <sys/stat.h>
#include "../../includes/core/criar_diretorio.h"
#include "../../includes/core/identifica_arquivos.h"

int command_start(void) {
    printf("Inicializando repositório VSR...\n");

    cria_diretorio(".vsr");
    cria_diretorio(".vsr/objects");
    cria_diretorio(".vsr/logs");

    char **filePaths = NULL;
    int totalArquivos = 0;
    
    identifica_arquivos(".", 0, &filePaths, &totalArquivos); 
    for (int i = 0; i < totalArquivos; i++) {
        printf("%s\n",filePaths[i]);
    }

    printf("Repositório iniciado com sucesso.\n");

    return 0;
}
