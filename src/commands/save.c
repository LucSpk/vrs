#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    int tamanhoContent = 10;
    int tamanhoAtualContent = 0;
    char **content = malloc(tamanhoContent);
    while (fgets(linha, sizeof(linha), fileIndex)) {
        hashAtual[0] = '\0';
        pathAtual[0] = '\0';

        sscanf(linha, "%s %s", hashAtual, pathAtual);
        char *hashBinaria = _converte_hash_para_binario(hashAtual, pathAtual);

        if(tamanhoAtualContent >= tamanhoContent) {
            tamanhoContent *= 2;
            char **contentTemp = realloc(content, tamanhoContent);

            content = contentTemp;
        }

        char *entry = malloc((sizeof(char *) * 9) + sizeof(pathAtual) + strlen(hashBinaria));
        sprintf(entry, "100644 %s\\0%s", pathAtual, hashBinaria);

        content[tamanhoAtualContent] = malloc(strlen(entry) + 1);
        strcpy(content[tamanhoAtualContent], entry);

        tamanhoAtualContent++;
    }
    

    // 2. Cria objeto tree para cada entrada do index.
    //      - Converter hash (hex) → binário
    //      - 100644 <path>\0<hash_binario>
    // 3. Montar conteúdo da tree
    //      - Junte tudo: <entry1><entry2><entry3>...
    // 4. Criar objeto tree completo
    //      - tree <tamanho>\0<conteudo>
    // 5. Gerar hash da tree
    //      - hash_tree = SHA(...)
    // 6. Salvar tree em .vrs/objects/
    //      - Mesma lógica do blob: objects/xx/yyyy...
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