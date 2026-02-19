#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>

static void _identificaArquivos(char *caminho, int exibeOcultos, char ***arr) {
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
             _identificaArquivos(caminhoCompleto, exibeOcultos, arr); // recursão
        } else if (S_ISREG(info.st_mode)) {
            printf("%s\n", caminhoCompleto);
        }
    }
    closedir(dir);
}

void identificaArquivos(char *caminho, int exibeOcultos, char ***arr) {
    
    _identificaArquivos(caminho, exibeOcultos, arr);
}