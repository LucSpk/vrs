#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../../includes/core/utils.h"

static char *_converte_hash_para_binario(char *hash, char *path) {
    char *result = malloc((strlen(hash) * 8) + 1);
    
    char output[sizeof(char *) + 1];
    int count = 0;
    while(*hash) {
        char_to_bin((unsigned char)*hash, output);
        result[count + 0] = output[0];
        result[count + 1] = output[1];
        result[count + 2] = output[2];
        result[count + 3] = output[3];
        result[count + 4] = output[4];
        result[count + 5] = output[5];
        result[count + 6] = output[6];
        result[count + 7] = output[7];
        count += 8;
        ++hash;
    }

    return result;
}

char *converte_hash_para_binario(char *hash, char *path) {
    return _converte_hash_para_binario(hash, path);
}