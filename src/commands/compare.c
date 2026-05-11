#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../includes/core/utils.h"

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
    
    char *caminho_hash_a = malloc(76);
    sprintf(caminho_hash_a, ".vsr/objects/%s/%s", extrair_substring(objeto_a, 0, 2), extrair_substring(objeto_a, 2, 62));
    FILE *fileCommitA = fopen(caminho_hash_a, "rb");

    char *caminho_hash_b = malloc(76);
    sprintf(caminho_hash_b, ".vsr/objects/%s/%s", extrair_substring(objeto_b, 0, 2), extrair_substring(objeto_b, 2, 62));
    FILE *fileCommitB = fopen(caminho_hash_b, "rb");

    printf("caminho_hash_a: %s\n", caminho_hash_a);
    printf("caminho_hash_b: %s\n", caminho_hash_b);
    printf("\n");

    // 2. Extrair suas trees
    //          Objeto commit: 		    "commit <tamanho>\0<conteúdo do commit>"
    fseek(fileCommitA, 0, SEEK_END);    // Move o cursor para o fim do file
    long tamanhoA = ftell(fileCommitA);  // Retorna posição atual do cursor, medida em bytes a partir do início do arquivo (tamanho do arquivo).
    rewind(fileCommitA);                // Retorna o cursor para o começo

    unsigned char *bufferA = malloc(tamanhoA);
    fread(bufferA, 1, tamanhoA, fileCommitA);

    // Repete para o objeto B
    fseek(fileCommitB, 0, SEEK_END);
    long tamanhoB = ftell(fileCommitB);
    rewind(fileCommitB);

    unsigned char *bufferB = malloc(tamanhoB);
    fread(bufferB, 1, tamanhoB, fileCommitB);

    printf("Buffer a: %s\n", bufferA);
    printf("Buffer b: %s\n", bufferB);
    printf("\n");


    // Pular header "commit <size>\0" 
    unsigned char *conteudoCommitA = bufferA;

    // achar o \0
    while (*conteudoCommitA != '\0') conteudoCommitA++;
        conteudoCommitA++; // pula o \0

    unsigned char *conteudoCommitB = bufferB;

    // achar o \0
    while (*conteudoCommitB != '\0') conteudoCommitB++;
        conteudoCommitB ++; // pula o \0

    
    printf("Conteudo do commit a: \n%s\n", conteudoCommitA);
    printf("Conteudo do commit b: \n%s\n", conteudoCommitB);
    printf("\n");

    char treeHashA[128];
    sscanf((char *)conteudoCommitA, "tree %s", treeHashA);
    printf("Tree hash A: %s\n", treeHashA);

    char treeHashB[128];
    sscanf((char *)conteudoCommitB, "tree %s", treeHashB);
    printf("Tree hash B: %s\n", treeHashB);

    char *caminho_tree_a = malloc(76);
    sprintf(caminho_tree_a, ".vsr/objects/%s/%s", extrair_substring(treeHashA, 0, 2), extrair_substring(treeHashA, 2, 62));
    FILE *fileTreeA = fopen(caminho_tree_a, "rb");

    char *caminho_tree_b = malloc(76);
    sprintf(caminho_tree_b, ".vsr/objects/%s/%s", extrair_substring(treeHashB, 0, 2), extrair_substring(treeHashB, 2, 62));
    FILE *fileTreeB = fopen(caminho_tree_b, "rb");


    //          Conteúdo do commit: 	"tree <hash da tree>\nparent <hash_commit_anterior>\nauthor <nome>\ndate <timestamp>\n\n<mensagem do commit>"
    //          Objeto tree: 		    "tree <tamanho>\0<conteudo da tree>"
    //          Conteúdo tree: 		    "<entry1><entry2><entry3>..."
    //          Entry: 			        "100644 <path>\0<hash_binario>"
    // 3. Converte hash para hex
    // 4. Comparar arquivos (path + hash)
    // 5. Mostrar:
    //      - added
    //      - removed
    //      - modified
    
    printf("Fim do compare\n");
    return 0;
}

int command_compare_simples_dois_objetos(char objeto_a[], char objeto_b[]) {
    return _command_compare_simples_dois_objetos(objeto_a, objeto_b);
}