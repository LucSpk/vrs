#include <stdio.h>
#include <stdlib.h>
#include "rastreador/leitor_de_arquivos/le_arquivo.h"

void identifica_arquivos(char *caminho, int exibeOcultos, char ***arr, int *tamanhoFinal); 
char *cria_hash_de_arquivo(const char *conteudo);
char *compactador_de_arquivos(const char *arquivo);
//void salva_arquivo(const char *caminho, const char *zipFile);

int main() {
    printf("Iniciando...\n");

    char **filePaths = NULL;
    int totalArquivos = 0;
    identifica_arquivos(".", 0, &filePaths, &totalArquivos); 
    
    for (int i = 0; i < totalArquivos; i++) {
        Arquivo arq = le_conteudo_arquivo(filePaths[i]);

        if (arq.conteudo != NULL) {
            char *hash;

            hash = cria_hash_de_arquivo(arq.conteudo);
            printf("%s\n", hash);
            char *zipFile = compactador_de_arquivos(arq.conteudo);
            
            //salva_arquivo(caminho, zipFile); 
            
            free(arq.conteudo);
            free(zipFile);
        }
    }
    
//    for (int i = 0; i < totalArquivos; i++) {
//        printf("%s\n", filePaths[i]);
//        free(filePaths[i]);   // libera cada string
//    }
//    free(filePaths);          // libera o array principal

    return 0;
}