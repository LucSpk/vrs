#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>

static void _cria_diretorios(const char *path) {
    char tmp[PATH_MAX];
    snprintf(tmp, sizeof(tmp), "%s", path);

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0777);
            *p = '/';
        }
    }

    mkdir(tmp, 0777);
}

static int _salva_arquivo_no_diretorio(const char *caminho, const char *fileName, const unsigned char *zipFile) {
    _cria_diretorios(caminho);
    
    int tamanho = strlen(caminho) + strlen(fileName) + 2;
    char filePath[tamanho];
    
    snprintf(filePath, sizeof(filePath), "%s/%s", caminho, fileName);
    
    // printf("Criando: %s\n", filePath);
    FILE *arquivo;
    arquivo = fopen(filePath, "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return 1;
    }

    fwrite(zipFile, 1, tamanho, arquivo);
    fclose(arquivo);
    return 0;
}

int salva_arquivo_no_diretorio(const char *caminho, const char *fileName, const unsigned char *zipFile) {
    return _salva_arquivo_no_diretorio(caminho, fileName, zipFile);
}