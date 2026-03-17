int _command_track_path(char *path) {

    // 1. Localizar o arquivo no working directory
    // 2. Verificar regras do .gitignore
    // 3. Ler metadados (path, permissões, timestamp)
    // 4. Ler conteúdo do arquivo
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

}

int command_track_path(char *path) {
    int _command_track_path(path);
}