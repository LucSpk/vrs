#include <stdio.h>

static int _command_change(char branchName[]) {
    // 1. Verifica se branch existe na ref
    // 2. Caso não exita lance um erro
    // 3. Caso exista altere o conteudo do HEAD para apontar a ref
    return 0;
}

int command_change(char branchName[]) {
    return _command_change(branchName);
}