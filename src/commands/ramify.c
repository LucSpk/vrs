#include <stdio.h>

static int _command_ramify(char branchName[]) {
    printf("Ramificacao criado com sucesso!");
    return 0;
}

int command_ramify(char branchName[]) {
    return _command_ramify(branchName);
}