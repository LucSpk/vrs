#ifndef ENTRY_TYPE
#define ENTRY_TYPE

typedef struct {
    char modo[8];
    char path[1024];
    unsigned char hash[32];
} Entry;

#endif