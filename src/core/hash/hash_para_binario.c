#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../../includes/core/utils.h"

static char *_converte_hash_para_binario(char *hash) {
    unsigned char *result = malloc(32);
    if (!result) return NULL;
    
    for (int i = 0; i < 32; i++) {
        sscanf(hash + (i * 2), "%2hhx", &result[i]);
    }
    
    return result;
}

char *converte_hash_para_binario(char *hash) {
    return _converte_hash_para_binario(hash);
}