#include <stdio.h>

void identificaArquivos(char *caminho, int exibeOcultos, char **arr); 

int main() {
    printf("Iniciando...\n");

    char **filePaths;
    identificaArquivos(".", 0, filePaths);   

    return 0;
}