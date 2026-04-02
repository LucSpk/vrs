#include <stdio.h>
#include <stdlib.h>

static int _command_save(char *path) {
    int err = 0;
    
    // 1. Lê arquivo index
    FILE *fileIndex = fopen("./.vsr/index", "r");
    if(fileIndex == NULL) {
        printf("Erro: Algo deu errado para ao abrir o arquivo index.");
        return 1;
    }

    char linha[1024];
    char hashAtual[128];
    char pathAtual[1024];

    while (fgets(linha, sizeof(linha), fileIndex)) {
        hashAtual[0] = '\0';
        pathAtual[0] = '\0';

        sscanf(linha, "%s %s", hashAtual, pathAtual);

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
    printf("\n");
}

int command_save(char *path) {
    return _command_save(path);
}