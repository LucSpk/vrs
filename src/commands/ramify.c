#include <stdio.h>

static int _command_ramify(char branchName[]) {
    // 1. Verifica se ramificação ja existe nos refs
    // 2. Caso exista dê um erro "Ramificação ja existe"
    // 3. Caso não exista crie a ref com o nome da nova branch e o commit anterior da branch atual
    // 4. Muda o ref para qual o HEAD aponta

    return 0;
}

int command_ramify(char branchName[]) {
    return _command_ramify(branchName);
}