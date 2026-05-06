#include <stdio.h>
#include <stdlib.h>

#include "../../includes/core/utils.h"

static int _command_compare_simples_dois_objetos(char objeto_a[], char objeto_b[]) {
    
    // 1. Lê os dois commits
    
    char *caminho_hash_a = malloc(76);
    sprintf(caminho_hash_a, ".vsr/objects/%s/%s", extrair_substring(objeto_a, 0, 2), extrair_substring(objeto_a, 2, 62));
    FILE *fileCommitA = fopen(caminho_hash_a, "rb");

    char *caminho_hash_b = malloc(76);
    sprintf(caminho_hash_b, ".vsr/objects/%s/%s", extrair_substring(objeto_b, 0, 2), extrair_substring(objeto_b, 2, 62));
    FILE *fileCommitB = fopen(caminho_hash_b, "rb");

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


    // Pular header "commit <size>\0" 
    unsigned char *conteudoCommitA = bufferA;

    // achar o \0
    while (*conteudoCommitA != '\0') conteudoCommitA++;
        conteudoCommitA++; // pula o \0

    unsigned char *conteudoCommitB = bufferB;

    // achar o \0
    while (*conteudoCommitB != '\0') conteudoCommitB++;
        conteudoCommitB ++; // pula o \0

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