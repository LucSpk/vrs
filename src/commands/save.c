#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../includes/core/hash.h"
#include "../../includes/core/utils.h"
#include "../../includes/core/io.h"

static void _move_memoria(unsigned char *target, unsigned char *source, size_t *offset, size_t length) {
    // O memcpy move um bloco de memorioa de um lugar para o outro
    // Sintaxe: void *memcpy(void *dest, const void *src, size_t n);
    // quando eu faço entry + offsetEntry eu estou movendo o "cursor" para o fim da memoria ocupada antes de inserir mais dados

    memcpy((target + *offset), source, length);           // modo
    *offset += length;
}

static size_t _cria_objeto(char *target, 
        char *header, size_t headerLen,
        char *arg1, size_t arg1Len,
        char *arg2, size_t arg2Len
    ) {
    size_t offset = 0;
    _move_memoria(target, header, &offset, headerLen);
    _move_memoria(target, arg1, &offset, arg1Len);
    target[offset] = '\0';                           // separador NULL
    offset += 1;
    _move_memoria(target, arg2, &offset, arg2Len);

    return offset;
}

static int _salva_objeto(char *hash, char * object, size_t objectSize) {
    char *caminho = malloc(14);
    sprintf(caminho, ".vsr/objects/%s", extrair_substring(hash, 0, 2));
    
    int err = salva_arquivo_no_diretorio(caminho, extrair_substring(hash, 2, 62), object, objectSize); 
    if(!err) 
        return err;

    printf("Erro ai salvar o arquivo no diretorio");
    return 1;
}

static int _command_save(char *mensagem) {
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
        unsigned char *hashBinaria = (unsigned char *) converte_hash_para_binario(hashAtual, pathAtual);
        
        size_t hashSize = 32;                               // - SHA-256 - 64 caracteres hex |2 caracteres hex = 1 byte| >> 32 bytes
        size_t pathLen = strlen(pathAtual);
        size_t entrySize = 7 + pathLen + 1 + hashSize;      // - "100644 " + path + '\0' + hash
        
        unsigned char *entry = malloc(entrySize);
        // size_t offsetEntry = 0;

        // 2. Cria objeto tree para cada entrada do index.
        //      - Converter hash (hex) → binário
        //      - 100644 <path>\0<hash_binario>
        _cria_objeto(entry, "100644 ", 7, pathAtual, pathLen, (char *)hashBinaria, hashSize);

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
    char tamanhoContentStr[20];
    sprintf(tamanhoContentStr, "%d", tamanhoContent);

    size_t sizeOfTamanhoContent = contar_digitos(tamanhoContent);
    unsigned int treeSize = sizeOfTamanhoContent + tamanhoContent + 6;
    
    unsigned char *tree = malloc(treeSize);

    size_t offsetTree = _cria_objeto(tree, "tree ", 5, tamanhoContentStr, sizeOfTamanhoContent, content, tamanhoContent);

    // 5. Gerar hash da tree
    //      - hash_tree = SHA(...)
    char *treeHash;
    treeHash = cria_hash(tree);

    // 6. Salvar tree em .vrs/objects/
    //      - Mesma lógica do blob: objects/xx/yyyy...
    // char *caminho = malloc(14);
    // sprintf(caminho, ".vsr/objects/%s", extrair_substring(treeHash, 0, 2));
    
    // err = salva_arquivo_no_diretorio(caminho, extrair_substring(treeHash, 2, 62), tree); 
    // if(err) {
    //     printf("Erro ai salvar o arquivo no diretorio");
    //     return 1;
    // }
    _salva_objeto(treeHash, tree, treeSize);

    // 7. Criar objeto commit
    //      - Formato:
    //          | tree <hash_tree>
    //          | parent <hash_commit_anterior>   (se existir)
    //          | author Seu Nome
    //          | date timestamp
    //          |
    //          | mensagem do commit
    
    FILE *headFile = fopen("./.vsr/HEAD", "r");
    if(headFile == NULL) {
        printf("Erro: Falha ao abrir arquivo HEAD");
        return 1;
    }
    
    char ref[128];
    fgets(ref, sizeof(ref), headFile);
    ref[strcspn(ref, "\n")] = '\0'; // remove \n
    
    char refPath[128];
    // %*s: Lê e ignora a primeira string
    sscanf(ref, "%*s %s", refPath);
    fclose(headFile);
    
    char completeRefPath[] = "./.vsr/";
    strcat(completeRefPath, refPath);

    FILE *refFile = fopen(completeRefPath, "r");
    if(refFile == NULL) {
        printf("Erro: Falha ao abrir arquivo ref");
        return 1;
    }

    char parentHash[128];
    fgets(parentHash, sizeof(parentHash), refFile);
    parentHash[strcspn(parentHash, "\n")] = '\0'; // remove \n
    fclose(refFile);

    // - Pega timeStamp do momento
    time_t timeStamp = time(NULL);

    // - Inicia conteudo do commit
    char commitContent[4096];
    if(strlen(parentHash) > 0) {
        sprintf(
            commitContent, 
            "tree %s\nparent %s\nauthor %s\nDate %ld\n\n%s",
            tree,
            parentHash,
            "", // TODO: Implementar melhoria para considerar o author
            timeStamp,
            mensagem
        );
    } else {
        sprintf(
            commitContent, 
            "tree %s\nauthor %s\nDate %ld\n\n%s",
            tree,
            "", // TODO: Implementar melhoria para considerar o author
            timeStamp,
            mensagem
        );
    }

    printf(commitContent);

    // 8. Cria objeto commit
    //      - commit <tamanho>\0<conteudo>
    size_t lenCommit = strlen(commitContent);
    char tamanhoCommitStr[32];
    sprintf(tamanhoCommitStr, "%lu", lenCommit);

    size_t lenTamanhoCommit = strlen(tamanhoCommitStr);
    size_t tamanhoHeaderCommit = 7 + lenTamanhoCommit + 1 + lenCommit;
    char *commit = malloc(tamanhoHeaderCommit);
    // size_t offsetCommit = 0;

    // memcpy(commitObject, "commit ", 7);
    // offsetCommit += 7;

    // memcpy(commitObject + offsetCommit, tamanhoCommitStr, lenTamanhoCommit);
    // offsetTree += lenTamanhoCommit;

    // commitObject[offsetCommit] = '\0';
    // offsetCommit += 1;

    // memcpy(commitObject + offsetCommit, commitContent, lenCommit);
    _cria_objeto(commit, "commit ", 7, tamanhoCommitStr, lenTamanhoCommit, commitContent, lenCommit);
    
    // 9. Gera hash do commit 
    char *commitHash;
    commitHash = cria_hash(commit);

    // 10. Salvar commit em .vrs/objects/
    // char *caminhoCommit = malloc(14);
    // sprintf(caminhoCommit, ".vsr/objects/%s", extrair_substring(commitHash, 0, 2));
    
    // err = salva_arquivo_no_diretorio(caminhoCommit, extrair_substring(commitHash, 2, 62), commit); 
    _salva_objeto(commitHash, commit, tamanhoHeaderCommit);

    // 11. Atualizar HEAD
    //      - Criar/atualizar: .vrs/HEAD
    //      - Conteúdo: <hash_do_commit>
    FILE *refFileWrite = fopen(completeRefPath, "w");
    if (refFileWrite == NULL) {
        printf("Erro ao abrir HEAD\n");
        return 1;
    }

    fprintf(refFileWrite, "%s", commitHash);
    fclose(refFileWrite);

    fclose(fileIndex);
}

int command_save(char *mensagem) {
    return _command_save(mensagem);
}