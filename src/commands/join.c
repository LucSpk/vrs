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

        *entries = realloc(*entries, sizeof(Entry) * (quantidade + 1));

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

static int _buscar_merge_base(char *commitA, char *commitB, char baseHash[]) {
    
    printf("\n\nConteudo commit Branch de Destino: %s\n", commitA);
    printf("Conteudo commit HEAD: %s\n", commitB);

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
    char baseHash[128];
    _buscar_merge_base(hashCommitBranchDestino, headHash, baseHash);

    // 5. Le os commits das branches
    long tamanhoCommitA;
    unsigned char *bufferCommitA = _ler_objeto(hashCommitBranchDestino, &tamanhoCommitA);

    long tamanhoCommitB;
    unsigned char *bufferCommitB = _ler_objeto(headHash, &tamanhoCommitB);
    
    if (!bufferCommitA || !bufferCommitB) {
        printf("Erro: Falha ao ler algum commit.\n");
        return 1;
    }

    unsigned char *conteudoCommitA = _pular_header(bufferCommitA);
    unsigned char *conteudoCommitB = _pular_header(bufferCommitB);

    return 0;

    // 6. Le as trees dos commits
    //    - Extrai hash das trees
    char treeHashA[65];
    char treeHashB[65];

    sscanf((char *)conteudoCommitA, "tree %64s", treeHashA);
    sscanf((char *)conteudoCommitB, "tree %64s", treeHashB);
    
    //    - Ler trees
    long tamanhoTreeA;
    unsigned char *bufferTreeA = _ler_objeto(treeHashA, &tamanhoTreeA);

    long tamanhoTreeB;
    unsigned char *bufferTreeB = _ler_objeto(treeHashB, &tamanhoTreeB);

    if (!bufferTreeA || !bufferTreeB) {
        return 1;
    }

    unsigned char *conteudoTreeA = _pular_header(bufferTreeA);
    unsigned char *conteudoTreeB = _pular_header(bufferTreeB);

    //    - Extrai tamanho real da tree
    size_t tamanhoRealTreeA = tamanhoTreeA - (conteudoTreeA - bufferTreeA);
    size_t tamanhoRealTreeB = tamanhoTreeB - (conteudoTreeB - bufferTreeB);

    //    - Faz o parse das entrys
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

    // 7. Compara os arquivos: novo, alterado e deletado
    //       arquivo novo	                    adiciona
    //       arquivo deletado                   apaga
    //       arquivo alterado só numa branch	aceita
    //       arquivo alterado nas duas	        conflito
    for (int i = 0; i < qtdA; i++) {
        Entry *entryB = _buscar_entry_por_path(entriesB, qtdB, entriesA[i].path);

        if(!entryB) {
            // Arquivo deletado em B
            printf("REMOVIDO em branch atual: %s\n", entriesA[i].path);
            continue;
        }

        if (memcmp(entriesA[i].hash, entryB->hash, 32) != 0) {
            // Arquivo alterado nas duas branches?
            // Aqui, para merge real, seria necessário buscar o ancestral comum para detectar conflito real.
            // Para simplificação: se diferente nas duas, marcamos como conflito.
            printf("CONFLITO: %s\n", entriesA[i].path);
        }
    }

    // Para cada entry em B (branch atual)
    for (int i = 0; i < qtdB; i++) {
        Entry *entryA = _buscar_entry_por_path(entriesA, qtdA, entriesB[i].path);
        if (!entryA) {
            // Arquivo novo em B
            printf("NOVO em branch atual: %s\n", entriesB[i].path);
        }
        // Se já existe em A, já foi tratado acima
    }

    // 8. Criar novo commit merge
    //        |  tree <nova_tree>
    //        |  parent <commit_main>
    //        |  parent <commit_branch>
    //      merge commit possui 2 parents
    // 9. Atualizar main
    return 0;
}

int command_join(char *destino) {
    return _command_join(destino);
}