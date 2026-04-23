#include <stdlib.h>
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

    char **staged = malloc(10);
    int tamanhoAtualStaged = 0;
    int tamanhoStaged = 0;

    char **modified = malloc(10);
    int tamanhoAtualModified = 0;
    int tamanhoModified = 0;
    
    char **untracked = malloc(10); 
    int tamanhoAtualUntracked = 0;
    int tamanhoUntracked = 0;

    while (fgets(linha, sizeof(linha), fileIndex)) {
        hash[0] = '\0';
        path[0] = '\0';
        status[0] = '\0';

        scanf(linha, "%s %s %s", hash, path, status);
        
        if(strcmp(status, "staged") == 0) {
            if(tamanhoAtualStaged >= tamanhoStaged) {
                tamanhoStaged *= 2;
                char **temp = realloc(staged, tamanhoStaged);
                if (!temp) {
                    perror("Erro no realloc");
                    return;
                }
                staged = temp;
            }

            int tamanhoResult = strlen(path) + 13;    // - "modified:   + <path> + \0"
            char result[tamanhoResult]; 
            sprintf(result, "modified:   %s", path);

            (*staged)[tamanhoStaged] = malloc(tamanhoResult);
            if(!(*staged)) {
                printf("ERRO: Falha na alocação de memória (malloc retornou NULL)\n");
                return 1;
            }

            sprintf((*staged)[tamanhoAtualStaged], result);
            tamanhoAtualStaged++;
        }
    }

    return 0;
}

int command_status() {
    return _command_status();
}