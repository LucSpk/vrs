#include <stdio.h>
#include "../../includes/core/le_arquivo.h"

void verifica(char *path) {
    // - TODO: Transforma em uma função separada.
    FILE *file;
    // const char *filename = "exemplo.txt";

    file = fopen(path, "r");
    if (file) {
        printf("O arquivo %s existe.\n", path);
        fclose(file); // Essencial fechar se abrir
    } else {
        printf("O arquivo %s nao existe.\n", path);
    }
}

static int _command_track_path(char *path) {
    
    // 1. Localizar o arquivo no working directory
    verifica(path);
    // 2. TODO: Verificar regras do .gitignore
    // 3. Ler metadados (path, permissões, timestamp)
    // 4. Ler conteúdo do arquivo
    Arquivo arquivo = le_conteudo_arquivo(path);
    // 5. Gerar hash do conteúdo (SHA)
    // 6. Verificar se o objeto já existe em .git/objects
    // 7. Se não existir:
    //     - Comprimir conteúdo (zlib)
    //     - Salvar como blob em .git/objects
    // 8. Criar/atualizar entrada no index (staging area):
    //     - caminho do arquivo
    //     - hash do blob
    //     - permissões
    // 9. Marcar arquivo como “staged”
    // 10. Atualizar estado interno para comparação futura (git status)
    return 0;
}

int command_track_path(char *path) {
    return _command_track_path(path);
}