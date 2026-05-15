#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../includes/core/io.h"
#include "../../includes/core/hash.h"
#include "../../includes/core/arquivos.h"
#include "../../includes/core/zipper.h"
#include "../../includes/core/utils.h"
#include "../../includes/core/identifica_arquivos.h"

static int _arquivo_existe(char *path) {
    return verifica(path); 
}

static Arquivo _le_arquivo(char *path) {
    return le_conteudo_arquivo(path);
}

static char *_criar_hash(Arquivo arquivo) {
    return cria_hash_de_arquivo(arquivo);
}

static int _arquivo_traqueado(char *hash, char *caminho) {
    int err = verifica_diretorio(caminho);
    if(err < 0) {
        printf("Erro: Diretorio não existe.\n");
        return 1;
    }

    char *fileName = extrair_substring(hash, 2, 62);
    int tamanhoFilePath = strlen(caminho) + strlen(fileName) + 2;
    char filePath[tamanhoFilePath];
    snprintf(filePath, sizeof(filePath), "%s/%s", caminho, fileName);
    if(!_arquivo_existe(filePath)) {
        printf("Arquivo não existe.\n");
        return 1;
    }
    return 0;
}

static int _atualiza_estado_arquivo(char *hash, char *path) {
    return atualiza_index(hash, path);
}

static int _comprime_salva(Arquivo arquivo, char *caminho, char *hash) {
    ZipperFile zipFile = compactador_de_arquivos(arquivo.conteudo);

    // - Cria o header do blob
    char header[64];
    int headerSize = snprintf(
        header,
        sizeof(header),
        "blob %ld",
        zipFile.tamanhoComprimido
    ) + 1; // inclui \0

    int tamanhoInput = headerSize + zipFile.tamanhoComprimido;
    unsigned char *fileParaSerSalvo = malloc(tamanhoInput);

    memcpy(fileParaSerSalvo, header, headerSize);       // - Adiciona o header
    memcpy(fileParaSerSalvo + headerSize,               // - Adiciona o conteudo do arquivo comprimido
        zipFile.conteudoComprimido,
        zipFile.tamanhoComprimido
    );
    
    //     - Salvar como blob em .git/objects
    int err =  salva_arquivo_no_diretorio(caminho, extrair_substring(hash, 2, 62), fileParaSerSalvo, tamanhoInput); 
    free(fileParaSerSalvo);
    return err;
}

static int _command_track_path(char *path) {
    int err = 0;
    
    // 1. Localizar o arquivo no working directory
    err = _arquivo_existe(path);
    if(err) {
        printf("Erro: Arquivo indicado não existe no local informado.\n");
        return 1;
    }
    
    // 2. TODO: Verificar regras do .gitignore
    // 3. Ler metadados (path, permissões, timestamp)
    // 4. Ler conteúdo do arquivo
    Arquivo arquivo = _le_arquivo(path);

    // 5. Gerar hash do conteúdo (SHA)
    char *hash = _criar_hash(arquivo);

    // 6. Verificar se o objeto já existe em .vrs/objects
    char *caminho = malloc(14);
    sprintf(caminho, ".vsr/objects/%s", extrair_substring(hash, 0, 2));

    err = _arquivo_traqueado(hash, caminho);
    /*
        Se o arquivo NÃO existir,
        precisamos salvar o blob.
    */
    if(err) {
        // 7. Se não existir:
        //     - Comprimir conteúdo (zlib)
        //     - Salvar como blob em .git/objects
        err = _comprime_salva(arquivo, caminho, hash);
        if(err) {
            printf("Erro: Falha ao salvar o arquivo no diretorio.\n");
            return 1;
        }
    }

    // 8. Criar/atualizar entrada no index (staging area):
    //     - caminho do arquivo
    //     - hash do blob
    //     - Marcar arquivo como “staged”
    err = _atualiza_estado_arquivo(hash, path);
    if(err) {
        printf("Erro: Arquivo ao atualizar o estado do arquivo.\n");
        return 1;
    }
    printf("Hash: %s\n", hash);
    // 10. Atualizar estado interno para comparação futura (git status)
    return 0;
}

static int _command_track_all() {
    char **filePaths = NULL;
    int totalArquivos = 0;
    identifica_arquivos(".", 0, &filePaths, &totalArquivos); 

    return 0;
}

int command_track_path(char *path) {
    return _command_track_path(path);
}

int command_track_all() {
    return _command_track_all();
}