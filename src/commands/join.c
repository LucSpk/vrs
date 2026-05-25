#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../includes/core/io.h"
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

static Entry *_buscar_entry_por_path(Entry *entries, int quantidade, char *path) {

    for (int i = 0; i < quantidade; i++) {

        if (strcmp(entries[i].path, path) == 0) {
            return &entries[i];
        }
    }

    return NULL;
}

static int _parse_tree(unsigned char *conteudoTree, size_t tamanhoTree, Entry **entries) {

    size_t offset = 0;
    int quantidade = 0;

    *entries = NULL;

    while (offset < tamanhoTree) {

        // precisa existir ao menos hash SHA256
        if ((offset + 32) > tamanhoTree) {
            break;
        }

        Entry *tmp = realloc(*entries, sizeof(Entry) * (quantidade + 1));
        if (!tmp) {
            printf("Erro realloc\n");
            free(*entries);
            return -1;
        }

        *entries = tmp;

        Entry *entry = &(*entries)[quantidade];

        memset(entry, 0, sizeof(Entry));

        // 1. Ler modo
        int i = 0;

        while (offset < tamanhoTree && conteudoTree[offset] != ' ') {
            entry->modo[i++] = conteudoTree[offset++];
        }

        entry->modo[i] = '\0';
        if (offset >= tamanhoTree) {
            break;
        }

        offset++; // pula espaço

        // 2. Ler path
        i = 0;

        while (offset < tamanhoTree && conteudoTree[offset] != '\0') {
            entry->path[i++] = conteudoTree[offset++];
        }

        entry->path[i] = '\0';
        if (offset >= tamanhoTree) {
            break;
        }

        offset++; // pula \0

        // 3. Ler hash binário
        if ((offset + 32) > tamanhoTree) {
            break;
        }

        memcpy(entry->hash, conteudoTree + offset, 32);

        offset += 32;

        quantidade++;
    }

    return quantidade;
}

static int _buscar_merge_base(char *commitA, char *commitB, char **baseHash) {
    if (!baseHash || !*baseHash) {
        printf("Erro: baseHash é NULL\n");
        return 1;
    }
    
    if(strcmp(commitA, commitB) == 0) {
        strcpy(*baseHash, commitA);
        return 0;
    }
    
    char currentCommitA[65];
    strcpy(currentCommitA, commitA);
    int commitBaseEncontrado = 0;
    do {

        char currentParentB[65];
        char currentB[65];
        strcpy(currentB, commitB);
        while (1) {
            long tamanhoCommitB;
            unsigned char *bufferCommitB = _ler_objeto(currentB, &tamanhoCommitB);
            if (!bufferCommitB) {
                break;
            }

            unsigned char *conteudoCommitB = _pular_header(bufferCommitB);

            char *parentLineB = strstr((char *)conteudoCommitB, "parent ");
            if (parentLineB == NULL) {
                free(bufferCommitB);
                break;
            } 
            sscanf(parentLineB, "parent %64s", currentParentB);
            free(bufferCommitB);

            if (strcmp(currentCommitA, currentParentB) == 0) {
                printf("Commit base encontrado: %s\n", currentParentB);
                strcpy(*baseHash, currentParentB);
                return 0;
            }

            strcpy(currentB, currentParentB);

        }

        long tamanhoCommitA;
        unsigned char *bufferCommitA = _ler_objeto(currentCommitA, &tamanhoCommitA);
        if (!bufferCommitA) {
            break;
        }

        unsigned char *conteudoCommitA = _pular_header(bufferCommitA);
        
        char *parentLineA = strstr((char *)conteudoCommitA, "parent ");
        if (parentLineA == NULL) {
            free(bufferCommitA);
            commitBaseEncontrado = -1;
            break;
        } 

        sscanf(parentLineA, "parent %64s", currentCommitA);
        free(bufferCommitA);

    } while(commitBaseEncontrado >= 0);
    
    printf("Commit base não encontrado.\n");
    return 1;
}

static int _hashes_iguais(unsigned char a[32], unsigned char b[32]) {
    return memcmp(a, b, 32) == 0;
}

static int _addEntry(Entry **array, int *tamanhoAtual, int *capacidade, Entry nova) {
    *capacidade = (*capacidade == 0) ? 1 : (*capacidade * 2);
    if (*tamanhoAtual >= *capacidade) {
        *capacidade *= 2;
        Entry *temp = realloc(*array, (*capacidade) * sizeof(Entry));
        if (!temp) {
            printf("Erro no realloc de aceitar.\n");
            return 1;
        }
        *array = temp;
    }

    (*array)[*tamanhoAtual] = nova; // copia a struct inteira
    (*tamanhoAtual)++;
    return 0;
}

static int _command_join(char *destino) {
    // 1. Verifica se a branch destino existe
    char path[256];
    snprintf(path, sizeof(path), "./.vsr/refs/heads/%s", destino);

    int err = verifica(path);
    if(err) {
        printf("Erro: Branch destino não existe.\n");
        return 1;
    }
    printf("Erro: Branch destino existe.\n");

    // 2. Pega o hash do ultimo commit da branch destino
    FILE *branchDestino = fopen(path, "r");
    if(branchDestino == NULL) {
        printf("Erro: Não foi possível aobri o arquivo: %s\n", path);
        return 1;
    }

    char hashCommitBranchDestino[128];
    fgets(hashCommitBranchDestino, sizeof(hashCommitBranchDestino), branchDestino);
    hashCommitBranchDestino[strcspn(hashCommitBranchDestino, "\n")] = '\0';
    fclose(branchDestino);

    // 3. Le o HEAD
    FILE *headFile = fopen("./.vsr/HEAD", "r");
    if(headFile == NULL) {
        printf("Erro: Falha ao abrir arquivo HEAD\n");
        return 1;
    }

    char ref[128];
    fgets(ref, sizeof(ref), headFile);
    ref[strcspn(ref, "\n")] = '\0';

    char refPath[128];
    sscanf(ref, "%*s %s", refPath);
    fclose(headFile);

    char completeRefPath[256];
    snprintf(
        completeRefPath,
        sizeof(completeRefPath),
        "./.vsr/%s",
        refPath
    );
    FILE *refFile = fopen(completeRefPath, "r");
    if(refFile == NULL) {
        printf("Erro: Falha ao abrir arquivo ref.\n");
        return 1;
    }

    // 4. Pega o hash do ultimo commit da HEAD
    char headHash[128];
    fgets(headHash, sizeof(headHash), refFile);
    headHash[strcspn(headHash, "\n")] = '\0';
    fclose(refFile);

    // - Pegar hash do commit base
    char *baseHash = malloc(65); 
    if (!baseHash) {
        printf("Erro malloc baseHash\n");
        return 1;
    }
    _buscar_merge_base(hashCommitBranchDestino, headHash, &baseHash);

    // 5. Le os commits das branches
    long tamanhoCommitA;
    unsigned char *bufferCommitA = _ler_objeto(hashCommitBranchDestino, &tamanhoCommitA);

    long tamanhoCommitB;
    unsigned char *bufferCommitB = _ler_objeto(headHash, &tamanhoCommitB);

    long tamanhoCommitBase;
    unsigned char *bufferCommitBase = _ler_objeto(baseHash, &tamanhoCommitBase);
    
    if (!bufferCommitA || !bufferCommitB || !bufferCommitBase) {
        printf("Erro: Falha ao ler algum commit.\n");
        return 1;
    }

    unsigned char *conteudoCommitA = _pular_header(bufferCommitA);
    unsigned char *conteudoCommitB = _pular_header(bufferCommitB);
    unsigned char *conteudoCommitBase = _pular_header(bufferCommitBase);

    // 6. Le as trees dos commits
    //    - Extrai hash das trees
    char treeHashA[65];
    char treeHashB[65];
    char treeHashBase[65];

    sscanf((char *)conteudoCommitA, "tree %64s", treeHashA);
    sscanf((char *)conteudoCommitB, "tree %64s", treeHashB);
    sscanf((char *)conteudoCommitBase, "tree %64s", treeHashBase);
    
    //    - Ler trees
    long tamanhoTreeA;
    unsigned char *bufferTreeA = _ler_objeto(treeHashA, &tamanhoTreeA);

    long tamanhoTreeB;
    unsigned char *bufferTreeB = _ler_objeto(treeHashB, &tamanhoTreeB);

    long tamanhoTreeBase;
    unsigned char *bufferTreeBase = _ler_objeto(treeHashBase, &tamanhoTreeBase);

    if (!bufferTreeA || !bufferTreeB || !bufferTreeBase) {
        return 1;
    }

    unsigned char *conteudoTreeA = _pular_header(bufferTreeA);
    unsigned char *conteudoTreeB = _pular_header(bufferTreeB);
    unsigned char *conteudoTreeBase = _pular_header(bufferTreeBase);

    //    - Extrai tamanho real da tree
    size_t tamanhoRealTreeA = tamanhoTreeA - (conteudoTreeA - bufferTreeA);
    size_t tamanhoRealTreeB = tamanhoTreeB - (conteudoTreeB - bufferTreeB);
    size_t tamanhoRealTreeBase = tamanhoTreeBase - (conteudoTreeBase - bufferTreeBase);

    //    - Faz o parse das entrys
    Entry *entriesA = NULL;
    Entry *entriesB = NULL;
    Entry *entriesBase = NULL;

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

    int qtdBase = _parse_tree(
        conteudoTreeBase,
        tamanhoRealTreeBase,
        &entriesBase
    );

    // 7. Compara os arquivos: novo, alterado e deletado
    //       arquivo novo	                    adiciona
    //       arquivo deletado                   apaga
    //       arquivo alterado só numa branch	aceita
    //       arquivo alterado nas duas	        conflito

    int tamanhoAceitar = 10;
    int tamanhoAtualAceitar = 0;
    Entry *aceitar = malloc(sizeof(Entry) * tamanhoAceitar);
    if (!aceitar) {
        printf("Erro malloc aceitar\n");
        return 1;
    }

    int tamanhoAdicionar = 10;
    int tamanhoAtualAdicionar = 0;
    Entry *adicionar = malloc(sizeof(Entry) * tamanhoAdicionar);
    if (!adicionar) {
        printf("Erro malloc adicionar\n");
        return 1;
    }

    int tamanhoRemover = 10;
    int tamanhoAtualRemover = 0;
    Entry *remover = malloc(sizeof(Entry) * tamanhoRemover);
    if (!remover) {
        printf("Erro malloc remover\n");
        return 1;
    }

    int tamanhoConflitos = 10;
    int tamanhoAtualConflitos = 0;
    Entry *conflitos = malloc(sizeof(Entry) * tamanhoConflitos);
    if (!conflitos) {
        printf("Erro malloc conflitos\n");
        return 1;
    }

    for(int i = 0; i < qtdBase; i++) {
        // A é o destino, B o atual, Base é o da intersecção 
        Entry *entryA = _buscar_entry_por_path(entriesA, qtdA, entriesBase[i].path);
        Entry *entryB = _buscar_entry_por_path(entriesB, qtdB, entriesBase[i].path);

        if(entryA && entryB) {
            if(_hashes_iguais(entriesBase[i].hash, entryA->hash)) {
                if(_hashes_iguais(entriesBase[i].hash, entryB->hash)) {
                    printf("IDENTICO em A e B - Ignora: %s\n", entriesBase[i].path);
                    continue;
                }

                printf("ALTERADO só em B, aceita B: %s\n", entriesBase[i].path);
                if(_addEntry(&aceitar, &tamanhoAtualAceitar, &tamanhoAceitar, *entryB)) {
                    printf("Erro ao adicionar entry na lista de aceitar.\n");
                    return 1; 
                }
                
                continue;
            }
            
            if(_hashes_iguais(entriesBase[i].hash, entryB->hash)) {
                printf("ALTERADO só em A, aceita A: %s\n", entriesBase[i].path);
                if(_addEntry(&aceitar, &tamanhoAtualAceitar, &tamanhoAceitar, *entryA)) {
                    printf("Erro ao adicionar entry na lista de aceitar.\n");
                    return 1; 
                }
                continue;
            }

            if(_hashes_iguais(entryA->hash, entryB->hash)) {
                printf("ALTERADO IGUAL, aceita: %s\n", entriesBase[i].path);
                if(_addEntry(&aceitar, &tamanhoAtualAceitar, &tamanhoAceitar, *entryA)) {
                    printf("Erro ao adicionar entry na lista de aceitar.\n");
                    return 1; 
                }
                continue;
            }

            printf("ALTERADO DIFERENTE em A e em B, Conflito: %s\n", entriesBase[i].path);
            if(_addEntry(&conflitos, &tamanhoAtualConflitos, &tamanhoConflitos, *entryA)) {
                printf("Erro ao adicionar entry na lista de aceitar.\n");
                return 1; 
            }
            continue;
        }

        if(entryA) {
            if(_hashes_iguais(entriesBase[i].hash, entryA->hash)) {
                printf("DELETADO só em B - Deleta: %s\n", entriesBase[i].path);
                if(_addEntry(&remover, &tamanhoAtualRemover, &tamanhoRemover, *entryA)) {
                    printf("Erro ao adicionar entry na lista de aceitar.\n");
                    return 1; 
                }
                continue;
            }

            printf("DELETADO em B e ALTERADO em A - Conflito: %s\n", entriesBase[i].path);
            if(_addEntry(&conflitos, &tamanhoAtualConflitos, &tamanhoConflitos, *entryA)) {
                printf("Erro ao adicionar entry na lista de aceitar.\n");
                return 1; 
            }
            continue;
        }

        if(entryB) {
            if(_hashes_iguais(entriesBase[i].hash, entryB->hash)) {
                printf("DELETADO só em A - Deleta: %s\n", entriesBase[i].path);
                if(_addEntry(&remover, &tamanhoAtualRemover, &tamanhoRemover, *entryB)) {
                    printf("Erro ao adicionar entry na lista de aceitar.\n");
                    return 1; 
                }
                continue;
            }

            printf("DELETADO em A e ALTERADO em B - Conflito: %s\n", entriesBase[i].path);
            if(_addEntry(&conflitos, &tamanhoAtualConflitos, &tamanhoConflitos, *entryB)) {
                printf("Erro ao adicionar entry na lista de aceitar.\n");
                return 1; 
            }
            continue;
        }

        printf("DELETADO em A e em B - Deleta: %s\n", entriesBase[i].path);
        if(_addEntry(&remover, &tamanhoAtualRemover, &tamanhoRemover, *entryB)) {
            printf("Erro ao adicionar entry na lista de aceitar.\n");
            return 1; 
        }
    }

    // - Novo na branch de destino
    for (int i = 0; i < qtdA; i++) {
        Entry *entryBase = _buscar_entry_por_path(entriesBase, qtdBase, entriesA[i].path);
        if(entryBase)
            continue;

        Entry *entryB = _buscar_entry_por_path(entriesB, qtdB, entriesA[i].path);
        if(!entryB) {
            printf("CRIADO só em A, adiciona A: %s\n", entriesA[i].path);
            if(_addEntry(&adicionar, &tamanhoAtualAdicionar, &tamanhoAdicionar, entriesA[i])) {
                printf("Erro ao adicionar entry na lista de aceitar.\n");
                return 1; 
            }
            continue;
        }

        if(_hashes_iguais(entriesA[i].hash, entryB->hash)) {
            printf("CRIADO IGUAL, aceita: %s\n", entriesA[i].path);
            if(_addEntry(&adicionar, &tamanhoAtualAdicionar, &tamanhoAdicionar, entriesA[i])) {
                printf("Erro ao adicionar entry na lista de aceitar.\n");
                return 1; 
            }
            continue;
        }

        printf("CRIADO DIFERENTE, conflito: %s\n", entriesA[i].path);
        if(_addEntry(&conflitos, &tamanhoAtualConflitos, &tamanhoConflitos, entriesA[i])) {
            printf("Erro ao adicionar entry na lista de aceitar.\n");
            return 1; 
        }
    }

    // - Novo na branch Atual
    for (int i = 0; i < qtdB; i++) {
        Entry *entryBase = _buscar_entry_por_path(entriesBase, qtdBase, entriesB[i].path);
        if(entryBase)
            continue;

        Entry *entryA = _buscar_entry_por_path(entriesA, qtdA, entriesB[i].path);
        if(!entryA) {
            printf("CRIADO só em B, adiciona B: %s\n", entriesB[i].path);
            if(_addEntry(&adicionar, &tamanhoAtualAdicionar, &tamanhoAdicionar, entriesB[i])) {
                printf("Erro ao adicionar entry na lista de aceitar.\n");
                return 1; 
            }
        }
    }

    // 8. Criar novo commit merge
    //        |  tree <nova_tree>
    //        |  parent <commit_main>
    //        |  parent <commit_branch>
    //      merge commit possui 2 parents
    // 9. Atualizar main

    free(aceitar);
    free(adicionar);
    free(remover);
    free(conflitos);

    free(bufferCommitA);
    free(bufferCommitB);
    free(bufferCommitBase);

    free(bufferTreeA);
    free(bufferTreeB);
    free(bufferTreeBase);

    free(entriesA);
    free(entriesB);
    free(entriesBase);

    free(baseHash);

    return 0;
}

int command_join(char *destino) {
    return _command_join(destino);
}