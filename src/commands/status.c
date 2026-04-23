#include <stdio.h>
#include <string.h>

int _command_status() {
    FILE *fileIndex = fopen(".vsr/index", "r");
    if(!fileIndex) {
        printf("ERRO: Erro ao abrir o arquivo index.");
        return 1;
    }

    char linha[1024];
    char hash[128];
    char path[1024];
    char status[32];

    while (fgets(linha, sizeof(linha), fileIndex)) {
        printf("%s\n", linha);
    }
    

    return 0;
}

int command_status() {
    return _command_status();
}