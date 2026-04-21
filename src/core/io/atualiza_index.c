#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../includes/core/io.h"

static int _atualiza_index(char *hash, char *fileName) {
    if(verifica("./.vsr/index")) {
        int err = 0;
        err = salva_arquivo_no_diretorio(".vsr/", "index", " ", 1); 
        if(err) {
            printf("Erro ao criar arquivo index.\n");
            return 1;
        }
    }

    FILE *file = fopen(".vsr/index", "r");
    if (file == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        return 1;
    }

    FILE *temp = fopen(".vsr/index-tmp", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        return 1;
    }

    char linha[1024];
    char hashAtual[128];
    char pathAtual[512];
    int existe = 0;
    
    while(fgets(linha, sizeof(linha), file)) {
        hashAtual[0] = '\0';
        pathAtual[0] = '\0';

        sscanf(linha, "%s %s", hashAtual, pathAtual);
        if(strcmp(pathAtual, fileName) == 0) {
            printf("Arquivo existe: %s\n", fileName);

            fprintf(temp, "%s %s\n", hash, fileName);
            existe = 1;
        } else {
            fprintf(temp, "%s %s\n", hashAtual, pathAtual);
        }
    }

    if(!existe) {
        fprintf(temp, "%s %s\n", hash, fileName);
    }

    fclose(file);
    fclose(temp);

    remove(".vsr/index");
    rename(".vsr/index-tmp", ".vsr/index");

    return 0;
}

int atualiza_index(char *hash, char *fileName) {
    return _atualiza_index(hash, fileName);
}