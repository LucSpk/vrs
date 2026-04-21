static char _char_to_bin(unsigned char c, char *out) {
    for (int i = 7; i >= 0; i--) {
        out[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
    out[8] = '\0';
}

char char_to_bin(unsigned char c, char *out) {
    return _char_to_bin(c, out);
}