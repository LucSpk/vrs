#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../includes/core/utils.h"
#include "../../includes/types/entry.h"

static unsigned char *_ler_objeto(char *hash, long *tamanho) {

    char caminho[128];

    snprintf(
        caminho,
        sizeof(caminho),
        ".vsr/objects/%s/%s",
        extrair_substring(hash, 0, 2),
        extrair_substring(hash, 2, 62)
    );

    FILE *file = fopen(caminho, "rb");
    if (!file) {
        printf("Erro ao abrir objeto: %s\n", caminho);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *tamanho = ftell(file);
    rewind(file);

    unsigned char *buffer = malloc(*tamanho);

    fread(buffer, 1, *tamanho, file);

    fclose(file);

    return buffer;
}

static unsigned char *_pular_header(unsigned char *buffer) {

    while (*buffer != '\0')
        buffer++;

    return buffer + 1;
}

static int _parse_tree(
    unsigned char *conteudoTree,
    size_t tamanhoTree,
    Entry **entries
) {

    size_t offset = 0;
    int quantidade = 0;

    *entries = NULL;

    while (offset < tamanhoTree) {

        *entries = realloc(
            *entries,
            sizeof(Entry) * (quantidade + 1)
        );

        Entry *entry = &(*entries)[quantidade];

        memset(entry, 0, sizeof(Entry));

        // 1. Ler modo
        int i = 0;

        while (conteudoTree[offset] != ' ') {
            entry->modo[i++] = conteudoTree[offset++];
        }

        entry->modo[i] = '\0';

        offset++; // pula espaço

        // 2. Ler path
        i = 0;

        while (conteudoTree[offset] != '\0') {
            entry->path[i++] = conteudoTree[offset++];
        }

        entry->path[i] = '\0';

        offset++; // pula \0

        // 3. Ler hash binário
        memcpy(entry->hash, conteudoTree + offset, 32);

        offset += 32;

        quantidade++;
    }

    return quantidade;
}

static int _command_compare_simples_dois_objetos(char objeto_a[], char objeto_b[]) {
    
    // 1. Lê os dois commits
    long tamanhoCommitA;
    unsigned char *bufferCommitA = _ler_objeto(objeto_a, &tamanhoCommitA);

    long tamanhoCommitB;
    unsigned char *bufferCommitB = _ler_objeto(objeto_b, &tamanhoCommitB);

    if (!bufferCommitA || !bufferCommitB) {
        printf("Erro: Falha ao ler algum commit.\n");
        return 1;
    }
    
    unsigned char *conteudoCommitA = _pular_header(bufferCommitA);
    unsigned char *conteudoCommitB = _pular_header(bufferCommitB);
    
    // 2. Extrai hash das trees
    char treeHashA[65];
    char treeHashB[65];

    sscanf((char *)conteudoCommitA, "tree %64s", treeHashA);
    sscanf((char *)conteudoCommitB, "tree %64s", treeHashB);

    // 3. Ler trees
    long tamanhoTreeA;
    unsigned char *bufferTreeA = _ler_objeto(treeHashA, &tamanhoTreeA);

    long tamanhoTreeB;
    unsigned char *bufferTreeB = _ler_objeto(treeHashB, &tamanhoTreeB);

    if (!bufferTreeA || !bufferTreeB) {
        return 1;
    }

    unsigned char *conteudoTreeA = _pular_header(bufferTreeA);
    unsigned char *conteudoTreeB = _pular_header(bufferTreeB);

    // 4. Extrai tamanho real da tree
    size_t headerTreeALen = conteudoTreeA - bufferTreeA;
    size_t tamanhoRealTreeA = tamanhoTreeA - headerTreeALen;

    size_t headerTreeBLen = conteudoTreeB - bufferTreeB;
    size_t tamanhoRealTreeB = tamanhoTreeB - headerTreeBLen;

    // 5. Faz o parse das entrys
    Entry *entriesA = NULL;
    Entry *entriesB = NULL;

    int qtdA = _parse_tree(
        conteudoTreeA,
        tamanhoRealTreeA,
        &entriesA
    );

    int qtdB = _parse_tree(
        conteudoTreeB,
        tamanhoRealTreeB,
        &entriesB
    );

    // 6. Faz a comparação

    
    
    printf("Fim do compare\n");
    return 0;
}

int command_compare_simples_dois_objetos(char objeto_a[], char objeto_b[]) {
    return _command_compare_simples_dois_objetos(objeto_a, objeto_b);
}