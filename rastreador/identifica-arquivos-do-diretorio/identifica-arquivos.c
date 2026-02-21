#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>

static void _identificaArquivos(char *caminho, int exibeOcultos, char ***arr, int *tamanhoPadrao, int *tamanhoAtual) {
    struct dirent *entrada;
    DIR *dir = opendir(caminho);

    if(!dir) {
        // Bem temos um problema aqui, algo deu errado e o diretório não existe..
        perror("Erro ao abrir o diretório que foi passado na declaração do método.");
        return;
    }
    
    while ((entrada = readdir(dir)) != NULL) {
        // - Fiz um teste e está aparecendo '.' e '..', na saida, o que corresponde a pasta atual e a aterior, vamos remover isso..
        if(strcmp(entrada -> d_name, ".") == 0 || strcmp(entrada -> d_name, "..") == 0) {
            continue;
        }

        // Se a flag estiver ativada, ignora arquivos que começam com "."
        if (!exibeOcultos && entrada->d_name[0] == '.') {
            continue;
        }

        char caminhoCompleto[PATH_MAX];
        // - Formata a string;
        snprintf(caminhoCompleto, sizeof(caminhoCompleto), "%s/%s", caminho, entrada -> d_name);

        struct stat info;
        if (stat(caminhoCompleto, &info) == -1) {
            perror("Erro no stat");
            continue;
        }
        
        // - Caso for um diretório, ele aciona a funcao recursivamente até encontrar o fim.
        if (S_ISDIR(info.st_mode)) {
             _identificaArquivos(caminhoCompleto, exibeOcultos, arr, tamanhoPadrao, tamanhoAtual);      // - Recursão
        } else if (S_ISREG(info.st_mode)) {
            if(*tamanhoAtual >= *tamanhoPadrao) {                     // - Se o tamanho total do array se aproximar do maximo de memória reservado, dobra o tamanho usando realloc
                (*tamanhoPadrao) *= 2;
                char **temp = realloc(*arr, (*tamanhoPadrao) * sizeof(char *));
                if (!temp) {
                    perror("Erro no realloc");
                    return;
                }
                *arr = temp;
            }
            //Posso ir para frente nos espeços de memória do ponteiro usanro arr[posição], dessa forma (*arr)[posição], me permite passear no segundo nivel do array
            (*arr)[*tamanhoAtual] = malloc(strlen(caminhoCompleto) + 1);     // - Aloca memmória suficiente para o caminho + caractere nulo final '\0'
            if (*arr == NULL) {
                fprintf(stderr, "Erro: Falha na alocação de memória (malloc retornou NULL)\n");
                return;
            }

            strcpy((*arr)[*tamanhoAtual], caminhoCompleto);

            (*tamanhoAtual)++;
            printf("%s\n", caminhoCompleto);
        }
    }
    closedir(dir);
}

void identificaArquivos(char *caminho, int exibeOcultos, char ***arr) {     // - ***arr eu tenho um ponteiro para o endereço de memória do array de strings

    int tamanhoAtual = 0;
    int tamanhoPadrao = 2;
    *arr = malloc(tamanhoPadrao * sizeof(char *));      // - Grava o endereço de memória dinâmica no ponteiro arr
                                                        //      sizeof(char *) retornar o tamanho máximo de um uma variável tipo 'char' em bytes. usar somente o sizeof(char) 
                                                        //      resultaria somente em tamanho 1 de um caracter apenas. "char *" representa o tamano do espaseo de memória do tipo char. 
                                                        //      multiplica pelo tamanho padrão, para que tenha a quantidade de bytes equivalente a 10 chars.
    if (!*arr) {
        perror("Erro no malloc inicial");
        return;
    }

    _identificaArquivos(caminho, exibeOcultos, arr, &tamanhoPadrao, &tamanhoAtual);
}