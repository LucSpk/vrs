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

    int tamanhoAtualStaged = 10;
    char **staged = malloc(sizeof(char *) * tamanhoAtualStaged);
    int tamanhoStaged = 0;

    int tamanhoAtualModified = 10;
    char **modified = malloc(sizeof(char *) * tamanhoAtualModified);
    int tamanhoModified = 0;
    
    int tamanhoAtualUntracked = 10;
    char **untracked = malloc(sizeof(char *) * tamanhoAtualUntracked); 
    int tamanhoUntracked = 0;

    while (fgets(linha, sizeof(linha), fileIndex)) {
        hash[0] = '\0';
        path[0] = '\0';
        status[0] = '\0';

        sscanf(linha, "%s %s %s", hash, path, status);
        
        if(strcmp(status, "staged") == 0) {
            if(tamanhoAtualStaged >= tamanhoStaged) {
                tamanhoStaged *= 2;
                char **temp = realloc(staged, sizeof(char *) * tamanhoStaged);
                if (!temp) {
                    perror("Erro no realloc");
                    return 1;
                }
                staged = temp;
            }

            int tamanhoResult = strlen(path) + 13;    // - "modified:   + <path> + \0"
            char result[tamanhoResult]; 
            sprintf(result, "modified:   %s", path);

            staged[tamanhoAtualStaged] = malloc(sizeof(char *) * tamanhoResult);
            if(!(*staged)) {
                printf("ERRO: Falha na alocação de memória (malloc retornou NULL)\n");
                return 1;
            }

            strcpy(staged[tamanhoAtualStaged], result);
            tamanhoAtualStaged++;
        }
    }
    

    return 0;
}

int command_status() {
    return _command_status();
}