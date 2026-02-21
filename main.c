#include <stdio.h>

void identificaArquivos(char *caminho, int exibeOcultos, char ***arr); 

int main() {
    printf("Iniciando...\n");

    char **filePaths = NULL;          // - Um ponteiro para um ponteiro de char
    identificaArquivos(".", 0, &filePaths);   

    return 0;
}