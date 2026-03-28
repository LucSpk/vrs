#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../includes/core/io.h"
#include "../../includes/core/hash.h"
#include "../../includes/core/arquivos.h"
#include "../../includes/core/zipper.h"
#include "../../includes/core/utils.h"

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

    // 6. Verificar se o objeto já existe em .vrs/objects
    char *caminho = malloc(14);
    sprintf(caminho, ".vsr/objects/%s", extrair_substring(hash, 0, 2));

    err = verifica_diretorio(caminho);
    if(err < 0) return 1;
    
    char *fileName = extrair_substring(hash, 2, 62);
    int tamanhoFilePath = strlen(caminho) + strlen(fileName) + 2;
    char filePath[tamanhoFilePath];
    snprintf(filePath, sizeof(filePath), "%s/%s", caminho, fileName);
    if(!verifica(filePath)) {
        printf("Arquivo ja existe.\n");
        return 1;
    }

    // 7. Se não existir:
    //     - Comprimir conteúdo (zlib)
    ZipperFile zipFile = compactador_de_arquivos(arquivo.conteudo);
    //     - Salvar como blob em .git/objects
    int tamanhoInput = (7 + 10 + zipFile.tamanhoComprimido);                 // - blob <size>\0dados
    char fileParaSerSalvo[tamanhoInput];
    snprintf(fileParaSerSalvo, tamanhoInput, "blob %ld\\0%s", zipFile.tamanhoComprimido, zipFile.conteudoComprimido);

    err = salva_arquivo_no_diretorio(caminho, extrair_substring(hash, 2, 62), fileParaSerSalvo); 
    if(err) {
        printf("Erro ai salvar o arquivo no diretorio");
        return 1;
    }
    // 8. Criar/atualizar entrada no index (staging area):
    //     - caminho do arquivo
    //     - hash do blob
    //     - permissões
    err = atualiza_index(hash, path);
    if(err) 
        return err;
    
    // 9. Marcar arquivo como “staged”
    // 10. Atualizar estado interno para comparação futura (git status)
    return 0;
}

int command_track_path(char *path) {
    return _command_track_path(path);
}