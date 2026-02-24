#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

void _cria_hash_de_arquivo(const char *conteudo) {
    unsigned char hash[SHA256_DIGEST_LENGTH]; // - SHA256_DIGEST_LENGTH = 32
    SHA256_CTX sha256;

    // Inicializa a estrutura de contexto
    SHA256_Init(&sha256);
    // Atualiza o contexto com os dados da string
    SHA256_Update(&sha256, conteudo, strlen(conteudo));
    // Finaliza e calcula o hash
    SHA256_Final(hash, &sha256);

    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    }
    hash_hex[64] = '\0'; // Finalizador da string

    printf("String: %s\n", conteudo);
    printf("SHA-256: %s\n", hash_hex);

    return;
}

void cria_hash_de_arquivo(const char *conteudo) {
    _cria_hash_de_arquivo(conteudo);
}