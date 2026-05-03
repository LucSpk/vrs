#include <stdio.h>

static int _command_change(char branchName[]) {

    return 0;
}

int command_change(char branchName[]) {
    return _command_change(branchName);
}