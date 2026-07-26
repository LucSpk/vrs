#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../includes/core/utils.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

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

int identificar_tipo_referencia(const char *valor) {
    if (valor == NULL || *valor == '\0') {
        return TIPO_REFERENCIA_INVALIDA;
    }

    size_t tamanho = strlen(valor);
    if (tamanho == 64) {
        for (size_t i = 0; i < tamanho; i++) {
            if (!isxdigit((unsigned char)valor[i])) {
                return TIPO_REFERENCIA_BRANCH;
            }
        }
        return TIPO_REFERENCIA_HASH;
    }

    return TIPO_REFERENCIA_BRANCH;
}

int resolver_referencia_para_hash(const char *referencia, char *saida, size_t tamanho) {
    // - Verifica se a referência é um hash válido
    if (referencia == NULL || saida == NULL || tamanho == 0) {
        return 1;
    }

    // - Se for um hash válido, copia diretamente para a saída
    if (identificar_tipo_referencia(referencia) == TIPO_REFERENCIA_HASH) {
        if (snprintf(saida, tamanho, "%s", referencia) >= (int)tamanho) {
            return 1;
        }
        return 0;
    }

    // - Se for uma referência inválida, retorna erro
    if (identificar_tipo_referencia(referencia) == TIPO_REFERENCIA_INVALIDA) {
        return 1;
    }

    // - Se for uma referência de branch, lê o arquivo correspondente
    char path[PATH_MAX];
    if (snprintf(path, sizeof(path), "./.vsr/refs/heads/%s", referencia) >= (int)sizeof(path)) {
        return 1;
    }

    // - Lê o conteúdo do arquivo da branch
    FILE *refFile = fopen(path, "r");
    if (refFile == NULL) {
        return 1;
    }

    // - Lê o hash do arquivo da branch
    char valor[129];
    if (fgets(valor, sizeof(valor), refFile) == NULL) {
        fclose(refFile);
        return 1;
    }

    // - Remove o caractere de nova linha, se presente
    fclose(refFile);
    valor[strcspn(valor, "\n")] = '\0';

    // - Copia o hash para a saída
    if (snprintf(saida, tamanho, "%s", valor) >= (int)tamanho) {
        return 1;
    }

    return 0;
}