#include <stdio.h>
#include <stdlib.h>

#include "../../includes/core/utils.h"

static int _command_compare_simples_dois_objetos(char objeto_a[], char objeto_b[]) {
    
    // 1. Lê os dois commits
    
    char *caminho_hash_a = malloc(14);
    sprintf(caminho_hash_a, ".vsr/objects/%s/%s", extrair_substring(objeto_a, 0, 2), extrair_substring(objeto_a, 2, 76));
    FILE *commit_a = fopen(caminho_hash_a, "");

    char *caminho_hash_b = malloc(14);
    sprintf(caminho_hash_b, ".vsr/objects/%s/%s", extrair_substring(objeto_b, 0, 2), extrair_substring(objeto_b, 2, 76));
    FILE *commit_b = fopen(caminho_hash_b, "");

    // 2. Extrair suas trees
    //          Objeto commit: 		"commit <tamanho>\0<conteúdo do commit>"
    //          Conteúdo do commit: 	"tree <hash da tree>\nparent <hash_commit_anterior>\nauthor <nome>\ndate <timestamp>\n\n<mensagem do commit>"
    //          Objeto tree: 		    "tree <tamanho>\0<conteudo da tree>"
    //          Conteúdo tree: 		"<entry1><entry2><entry3>..."
    //          Entry: 			    "100644 <path>\0<hash_binario>"
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