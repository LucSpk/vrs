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

static int _salva_arquivo_no_diretorio(const char *caminho, const char *fileName, const unsigned char *content, size_t contentSize) {
    _cria_diretorios(caminho);
    
    int tamanho = strlen(caminho) + strlen(fileName) + 2;
    char filePath[tamanho];
    
    snprintf(filePath, sizeof(filePath), "%s/%s", caminho, fileName);
  
    FILE *arquivo;
    arquivo = fopen(filePath, "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return 1;
    }

    fwrite(content, 1, contentSize, arquivo);
    fclose(arquivo);
    return 0;
}

int salva_arquivo_no_diretorio(const char *caminho, const char *fileName, const unsigned char *content, size_t contentSize) {
    return _salva_arquivo_no_diretorio(caminho, fileName, content, contentSize);
}