#ifndef UTILS
#define UTILS

enum TipoReferencia {
    TIPO_REFERENCIA_INVALIDA = 0,
    TIPO_REFERENCIA_HASH = 1,
    TIPO_REFERENCIA_BRANCH = 2
};

char* extrair_substring(const char* str, int inicio, int tamanho);
char char_to_bin(unsigned char c, char *out);
int contar_digitos(int n);
int identificar_tipo_referencia(const char *valor);
int resolver_referencia_para_hash(const char *referencia, char *saida, size_t tamanho);

#endif