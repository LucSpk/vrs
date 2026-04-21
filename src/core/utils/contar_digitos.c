static int _contar_digitos(int n) {
    if (n == 0) return 1; // Caso especial para 0
    
    int contador = 0;
    // // Se quiser contar o sinal negativo, trate n < 0 separadamente
    // if (n < 0) n = -n; 

    while (n != 0) {
        n /= 10;
        contador++;
    }
    return contador;
}

int contar_digitos(int n) {
    return _contar_digitos(n);
}