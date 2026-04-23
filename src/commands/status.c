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

    int tamanhoStaged = 10;
    char **staged = malloc(sizeof(char *) * tamanhoStaged);
    int tamanhoAtualStaged = 0;

    int tamanhoModified = 10;
    char **modified = malloc(sizeof(char *) * tamanhoModified);
    int tamanhoAtualModified = 0;
    
    int tamanhoUntracked = 10;
    char **untracked = malloc(sizeof(char *) * tamanhoUntracked); 
    int tamanhoAtualUntracked = 0;

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
                    printf("Erro no realloc de staged.\n");
                    return 1;
                }
                staged = temp;
            }

            int tamanhoResult = strlen(path) + 11;    // - "staged:   + <path> + \0"
            char result[tamanhoResult]; 
            sprintf(result, "staged:   %s", path);

            staged[tamanhoAtualStaged] = malloc(sizeof(char *) * tamanhoResult);
            if(!(*staged)) {
                printf("ERRO: Falha na alocação de memória (malloc retornou NULL)\n");
                return 1;
            }

            strcpy(staged[tamanhoAtualStaged], result);
            tamanhoAtualStaged++;
        }

        if(strcmp(status, "modified") == 0) {
            if(tamanhoAtualModified >= tamanhoModified) {
                tamanhoModified *= 2;
                char **temp = realloc(modified, sizeof(char *) * tamanhoModified);
                if (!temp) {
                    printf("Erro no realloc de modified.\n");
                    return 1;
                }
                modified = temp;

            }

            int tamanhoResult = strlen(path) + 13;    // - "modified:   + <path> + \0"
            char result[tamanhoResult]; 
            sprintf(result, "modified:   %s", path);

            modified[tamanhoAtualModified] = malloc(sizeof(char *) * tamanhoResult);
            if(!(*modified)) {
                printf("ERRO: Falha na alocação de memória (malloc retornou NULL)\n");
                return 1;
            }

            strcpy(modified[tamanhoAtualModified], result);
        
            tamanhoAtualModified++;
        }
    }

    for(int i = 0; i < tamanhoAtualStaged; i++) {
        printf("%s\n", staged[i]);
    }

    for(int i = 0; i < tamanhoAtualModified; i++) {
        printf("%s\n", modified[i]);
    }

    return 0;
}

int command_status() {
    return _command_status();
}