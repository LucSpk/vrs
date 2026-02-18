#include <stdio.h>

void identificaArquivos(char *caminho, int exibeOcultos); 

int main() {
    printf("Iniciando...\n");

    identificaArquivos(".", 0);   
    return 0;
}