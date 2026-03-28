#include <stdio.h>
#include <dirent.h>
#include <errno.h>

static int _verifica_diretorio(char *dirPath) {
    DIR *dir = opendir(dirPath);
    if(!dir) {
        if(errno == ENOENT) {
            printf("Diretório não existe: %s\n", dirPath);
            return 1;
        }
        perror("Erro ao abrir diretório");
        return -1;
    }

    return 0;
}

int verifica_diretorio(char *dirPath) {
    return _verifica_diretorio(dirPath);
}