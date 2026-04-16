#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/core/hash.h"
#include "../../includes/core/utils.h"
#include "../../includes/core/io.h"

static char _char_to_bin(unsigned char c, char *out) {
    for (int i = 7; i >= 0; i--) {
        out[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
    out[8] = '\0';
}

static char *_converte_hash_para_binario(char *hash, char *path) {
    char *result = malloc((strlen(hash) * 8) + 1);
    
    char output[sizeof(char *) + 1];
    int count = 0;
    while(*hash) {
        _char_to_bin((unsigned char)*hash, output);
        result[count + 0] = output[0];
        result[count + 1] = output[1];
        result[count + 2] = output[2];
        result[count + 3] = output[3];
        result[count + 4] = output[4];
        result[count + 5] = output[5];
        result[count + 6] = output[6];
        result[count + 7] = output[7];
        count += 8;
        ++hash;
    }

    return result;
}

static int _contar_digitos(int n) {
    if (n == 0) return 1; // Caso especial para 0
    
    int contador = 0;
    // // Se quiser contar o sinal negativo, trate n < 0 separadamente
    // if (n < 0) n = -n; 

    while (n != 0) {
        n /= 10;
        contador++;
    }
    return contador;
}

static int _command_save(char *path) {
    int err = 0;
    
    // 1. Lê arquivo indexbrad
    FILE *fileIndex = fopen("./.vsr/index", "r");
    if(fileIndex == NULL) {
        printf("Erro: Algo deu errado para ao abrir o arquivo index.");
        return 1;
    }

    char linha[1024];
    char hashAtual[128];
    char pathAtual[1024];

    int tamanhoContent = 0;
    unsigned char *content = NULL;
    while (fgets(linha, sizeof(linha), fileIndex)) {
        hashAtual[0] = '\0';
        pathAtual[0] = '\0';

        sscanf(linha, "%s %s", hashAtual, pathAtual);
        unsigned char *hashBinaria = (unsigned char *) _converte_hash_para_binario(hashAtual, pathAtual);
        
        size_t hashSize = 32;                               // - SHA-256 - 64 caracteres hex |2 caracteres hex = 1 byte| >> 32 bytes
        size_t pathLen = strlen(pathAtual);
        size_t entrySize = 7 + pathLen + 1 + hashSize;      // - "100644 " + path + '\0' + hash
        
        unsigned char *entry = malloc(entrySize);
        size_t offsetEntry = 0;

        // 2. Cria objeto tree para cada entrada do index.
        //      - Converter hash (hex) → binário
        //      - 100644 <path>\0<hash_binario>
        memcpy(entry + offsetEntry, "100644 ", 7);           // modo
        offsetEntry += 7;
        memcpy(entry + offsetEntry, pathAtual, pathLen);     // path
        offsetEntry += pathLen;
        entry[offsetEntry] = '\0';                           // separador NULL
        offsetEntry += 1;
        memcpy(entry + offsetEntry, hashBinaria, hashSize);  // hash binário
        // O memcpy move um bloco de memorioa de um lugar para o outro
        // Sintaxe: void *memcpy(void *dest, const void *src, size_t n);
        // quando eu faço entry + offsetEntry eu estou movendo o "cursor" para o fim da memoria ocupada antes de inserir mais dados

        // 3. Montar conteúdo da tree
        //      - Junte tudo: <entry1><entry2><entry3>...
        unsigned char *tempContent = realloc(content, tamanhoContent + entrySize);
        if(!tempContent) {
            free(tempContent);
            free(entry);
            return 1;
        }

        content = tempContent;
        memcpy(content + tamanhoContent, entry, entrySize);
        tamanhoContent += entrySize;

        free(entry);
    }

    // 4. Criar objeto tree completo
    //      - tree <tamanho>\0<conteudo>
    size_t sizeOfTamanhoContent = _contar_digitos(tamanhoContent);
    unsigned int treeSize = sizeOfTamanhoContent + tamanhoContent + 6;
    unsigned char *tree = malloc(treeSize);
    size_t offsetTree = 0;

    memcpy(tree, "tree ", 5);
    offsetTree += 5;
    memcpy(tree + offsetTree, &tamanhoContent, sizeOfTamanhoContent);
    offsetTree += sizeOfTamanhoContent;
    tree[offsetTree] = '\0';
    offsetTree += 1;
    memcpy(tree + offsetTree, content, tamanhoContent);

    // 5. Gerar hash da tree
    //      - hash_tree = SHA(...)
    char *treeHash;
    treeHash = cria_hash(tree);

    // 6. Salvar tree em .vrs/objects/
    //      - Mesma lógica do blob: objects/xx/yyyy...
    char *caminho = malloc(14);
    sprintf(caminho, ".vsr/objects/%s", extrair_substring(treeHash, 0, 2));
    
    err = salva_arquivo_no_diretorio(caminho, extrair_substring(treeHash, 2, 62), tree); 
    if(err) {
        printf("Erro ai salvar o arquivo no diretorio");
        return 1;
    }

    // 7. Criar objeto commit
    //      - Formato:
    //          | tree <hash_tree>
    //          | parent <hash_commit_anterior>   (se existir)
    //          | author Seu Nome
    //          | date timestamp
    //          |
    //          | mensagem do commit
    // 8. Gerar hash do commit
    //      - commit <tamanho>\0<conteudo>
    // 9. Salvar commit em .vrs/objects/
    // 10. Atualizar HEAD
    //      - Criar/atualizar: .vrs/HEAD
    //      - Conteúdo: <hash_do_commit>
    fclose(fileIndex);
    printf("\n");
}

int command_save(char *path) {
    return _command_save(path);
}