#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../../includes/core/utils.h"

static char *_converte_hash_para_binario(char *hash) {
    unsigned char *result = malloc(32);  // SHA256 = 32 bytes
    if (!result) return NULL;
    
    // Processa 2 caracteres hex de cada vez
    for (int i = 0; i < 32; i++) {
        char hexPair[3];
        hexPair[0] = hash[i * 2];      // '6'
        hexPair[1] = hash[i * 2 + 1];  // 'a'
        hexPair[2] = '\0';
        
        result[i] = (unsigned char)strtol(hexPair, NULL, 16);  // 0x6a
    }
    
    return result;
}

char *converte_hash_para_binario(char *hash) {
    return _converte_hash_para_binario(hash);
}