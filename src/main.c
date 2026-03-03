#include <stdio.h>
#include <stdlib.h>
#include "rastreador/leitor_de_arquivos/le_arquivo.h"
#include <string.h>
#include <linux/limits.h>

void identifica_arquivos(char *caminho, int exibeOcultos, char ***arr, int *tamanhoFinal); 
char *cria_hash_de_arquivo(const char *conteudo);
char *compactador_de_arquivos(const char *arquivo);
void salva_arquivo_no_diretorio(const char *caminho, const char *fileName, const char *zipFile);

const char *pega_nome_arquivo(const char *path) {
    const char *p = strrchr(path, '/');
    return (p != NULL) ? p + 1 : path;
}

char* extrairSubstring(const char* str, int inicio, int tamanho) {
    if (str == NULL || inicio < 0 || tamanho < 0) return NULL;
    
    char* sub = (char*)malloc(tamanho + 1); // +1 para o '\0'
    if (sub == NULL) return NULL;
    
    strncpy(sub, str + inicio, tamanho);
    sub[tamanho] = '\0'; // Garantir que a string termina
    return sub;
}

int main() {
    printf("Iniciando...\n");

    char **filePaths = NULL;
    int totalArquivos = 0;
    identifica_arquivos(".", 0, &filePaths, &totalArquivos); 
    
    for (int i = 0; i < totalArquivos; i++) {
        
        Arquivo arq = le_conteudo_arquivo(filePaths[i]);
        if (arq.conteudo == NULL) 
            continue;    

        char *hash;
        
        char *objeto = malloc(28 + arq.tamanho);
        sprintf(objeto, "blob %ld\\0%s", arq.tamanho, arq.conteudo);
        hash = cria_hash_de_arquivo(objeto);

        char *zipFile = compactador_de_arquivos(arq.conteudo);
        
        
        char *caminho = malloc(14);
        sprintf(caminho, "./objects/%s", extrairSubstring(hash, 0, 2));
        
        salva_arquivo_no_diretorio(caminho, extrairSubstring(hash, 2, 62), zipFile); 
        
        char tmp[PATH_MAX];
        snprintf(tmp, sizeof(tmp), "%s/%s", caminho, extrairSubstring(hash, 2, 62));
        Arquivo arq2 = le_conteudo_arquivo(tmp);
        

        free(arq.conteudo);
        free(zipFile);
    }
    
//    for (int i = 0; i < totalArquivos; i++) {
//        printf("%s\n", filePaths[i]);
//        free(filePaths[i]);   // libera cada string
//    }
//    free(filePaths);          // libera o array principal

    return 0;
}