#include <stdlib.h>

static char* _extrair_substring(const char* str, int inicio, int tamanho) {
    if (str == NULL || inicio < 0 || tamanho < 0) return NULL;
    
    char* sub = (char*)malloc(tamanho + 1); // +1 para o '\0'
    if (sub == NULL) return NULL;
    
    strncpy(sub, str + inicio, tamanho);
    sub[tamanho] = '\0'; // Garantir que a string termina
    return sub;
}

char* extrair_substring(const char* str, int inicio, int tamanho) {
    return _extrair_substring(str, inicio, tamanho);
}