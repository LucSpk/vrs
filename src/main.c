#include <stdio.h>
#include <stdlib.h>

void identifica_arquivos(char *caminho, int exibeOcultos, char ***arr, int *tamanhoFinal); 
void le_conteudo_arquivo(char *filePath);

int main() {
    printf("Iniciando...\n");

    char **filePaths = NULL;          // - Um ponteiro para um ponteiro de char
    int totalArquivos = 0;
    identifica_arquivos(".", 0, &filePaths, &totalArquivos); 

    printf("\nArquivos encontrados: %d\n\n", totalArquivos);

    for (int i = 0; i < totalArquivos; i++) {
        printf("File Path: %s              - ", filePaths[i]);
        le_conteudo_arquivo(filePaths[i]);
    }

    
//    for (int i = 0; i < totalArquivos; i++) {
//        printf("%s\n", filePaths[i]);
//        free(filePaths[i]);   // libera cada string
//    }
//    free(filePaths);          // libera o array principal

    return 0;
}