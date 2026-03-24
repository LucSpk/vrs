#include <stdio.h>
#include "../../includes/core/io.h"
#include "../../includes/core/hash.h"
#include "../../includes/core/arquivo.h"
#include "../../includes/core/zipper.h"

static int _command_track_path(char *path) {
    
    // 1. Localizar o arquivo no working directory
    int err = 0;
    err = verifica(path);
    if(err) 
        return 1;
    
    // 2. TODO: Verificar regras do .gitignore
    // 3. Ler metadados (path, permissões, timestamp)
    // 4. Ler conteúdo do arquivo
    Arquivo arquivo = le_conteudo_arquivo(path);

    // 5. Gerar hash do conteúdo (SHA)
    char *hash;
    hash = cria_hash(arquivo);

    // 6. Verificar se o objeto já existe em .git/objects
    char *caminho = malloc(14);
    sprintf(caminho, "./objects/%s", extrair_substring(hash, 0, 2));

    err = verifica_diretorio(caminho);
    if(err < 0) return 1;
    
    char fileName = extrairSubstring(hash, 2, 62);
    int tamanho = strlen(caminho) + strlen(fileName) + 2;
    char filePath[tamanho];
    snprintf(filePath, sizeof(filePath), "%s/%s", caminho, fileName);
    if(!verifica(filePath)) {
        printf("Arquivo ja existe.");
        return 1;
    }

    // 7. Se não existir:
    //     - Comprimir conteúdo (zlib)
    ZipperFile zipFile = compactador_de_arquivos(arquivo.conteudo);
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