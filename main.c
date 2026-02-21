#include <stdio.h>
#include <stdlib.h>

void identificaArquivos(char *caminho, int exibeOcultos, char ***arr, int *tamanhoFinal); 

int main() {
    printf("Iniciando...\n");

    char **filePaths = NULL;          // - Um ponteiro para um ponteiro de char
    int totalArquivos = 0;
    identificaArquivos(".", 0, &filePaths, &totalArquivos); 

    printf("\nArquivos encontrados: %d\n\n", totalArquivos);

    for (int i = 0; i < totalArquivos; i++) {
        printf("%s\n", filePaths[i]);
        free(filePaths[i]);   // libera cada string
    }

    free(filePaths);          // libera o array principal

    return 0;
}