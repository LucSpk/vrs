#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void identificaArquivos(char *caminho) {
    printf("Localizando arquivos para: %s\n", caminho);

    struct dirent *entrada;
    DIR *dir = opendir(caminho);

    if(!dir) {
        // Bem temos um problema aqui, algo deu errado e o diretório não existe..
        perror("Erro ao abrir o diretório que foi passado na declaração do método.");
        return;
    }

    printf("Conteúdo do diretório:\n");
    
    while ((entrada = readdir(dir)) != NULL) {
        printf("%s\n", entrada->d_name);
    }
    closedir(dir);
}