#include <stdio.h>

static int _verifica(char *path) {
    FILE *file;

    file = fopen(path, "r");
    if (file) {
        fclose(file); // Essencial fechar se abrir
        return 0;
    } 

    printf("O arquivo %s nao existe.\n", path);
    return 1;
}

int verifica(char *path) {
    return _verifica(path);
}
