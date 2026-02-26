#include <stdio.h>
#include <stdlib.h>
#include "rastreador/leitor_de_arquivos/le_arquivo.h"

void identifica_arquivos(char *caminho, int exibeOcultos, char ***arr, int *tamanhoFinal); 
void cria_hash_de_arquivo(const char *conteudo);
void compactador_de_arquivos(const char *arquivo);

int main() {
    printf("Iniciando...\n");

    char **filePaths = NULL;          // - Um ponteiro para um ponteiro de char
    int totalArquivos = 0;
    identifica_arquivos(".", 0, &filePaths, &totalArquivos); 
    
    for (int i = 0; i < totalArquivos; i++) {
        printf("File Path: %s - ", filePaths[i]);
        Arquivo arq = le_conteudo_arquivo(filePaths[i]);

        if (arq.conteudo != NULL) {
            //printf("Tamanho: %ld bytes\n", arq.tamanho);
            //printf("Conteúdo:\n%s\n", arq.conteudo);
            cria_hash_de_arquivo(arq.conteudo);
            compactador_de_arquivos(arq.conteudo);
            free(arq.conteudo); // MUITO IMPORTANTE
        }
    }
    
//    for (int i = 0; i < totalArquivos; i++) {
//        printf("%s\n", filePaths[i]);
//        free(filePaths[i]);   // libera cada string
//    }
//    free(filePaths);          // libera o array principal

    return 0;
}