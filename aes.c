#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "random.h"

/* ========================= AES-256 KEY EXPANSION ========================= */

/* S-box oficial AES */
static const unsigned char sbox[256] = {
    /* 0     1    2    3    4    5    6    7    8    9    A    B    C    D    E    F */
    0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
    0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
    0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
    0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
    0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
    0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
    0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
    0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
    0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
    0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
    0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
    0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
    0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
    0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
    0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
    0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16
};

/* Rcon (solo primer byte cambia) */
static const unsigned char rcon[15] = {
    0x00, /* no se usa */
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36,
    0x6C,0xD8,0xAB,0x4D
};

/* RotWord: ABCD -> BCDA */
static void RotWord(unsigned char w[4]) {
    unsigned char tmp = w[0];
    w[0]=w[1];
    w[1]=w[2];
    w[2]=w[3];
    w[3]=tmp;
}

/* SubWord: aplica S-box byte a byte */
static void SubWord(unsigned char w[4]) {
    w[0] = sbox[w[0]];
    w[1] = sbox[w[1]];
    w[2] = sbox[w[2]];
    w[3] = sbox[w[3]];
}

typedef struct{
    unsigned char roundKey[16];
}roundKey;

/* generan la aes key*/
void genKey(unsigned char *aes,int size);
void printKey(unsigned char *aes,int size);

/* expande la key */
void genKeyRound(unsigned char *aes,int size,roundKey keys[15]);

/* cifra con aes */
void AddRoundKey(unsigned char state[16], const unsigned char rk[16]);
void SubBytes(unsigned char state[16]);
void ShiftRows(unsigned char state[16]);
void MixColumns(unsigned char state[16]);
/* orden de cifrado */
void Aes256EncryptBlock(unsigned char state[16], roundKey keys[15]);

int main(int argc,char *argv[]){
    unsigned char aesKey[32];
    roundKey keys[15];

    /* message "plain text" */
    unsigned char state[16] = {
    0x00,0x11,0x22,0x33,
    0x44,0x55,0x66,0x77,
    0x88,0x99,0xAA,0xBB,
    0xCC,0xDD,0xEE,0xFF
    };

    int r,j;

    srand(time(NULL));

    /* generamos la key random */
    memset(aesKey,'\0',sizeof(aesKey));
    genKey(aesKey,sizeof(aesKey));
    printKey(aesKey,sizeof(aesKey));

    /* expandimos la key en */
    genKeyRound(aesKey,sizeof(aesKey),keys);
    for (r=0; r<15; r++){
        printf("RK %d: ", r);
        for (j=0; j<16; j++){
            printf("%02X", keys[r].roundKey[j]);
            if (j<15) printf("-");
        }
        printf("\n");
    }

    /* mostramos el bloque plano */
    printf("Plain block:\n");
    printKey(state, 16);

    /* encriptamos el bloque */
    Aes256EncryptBlock(state, keys);

    /* mostramos el bloque cifrado */
    printf("Encrypted block:\n");
    printKey(state, 16);


    return 0;
}

void printKey(unsigned char *aes,int size){
    unsigned short i;

    for (i=0;i<size;i++){
        printf("%02X-",(unsigned char)aes[i]);
    }
    printf("\n");
}

void genKey(unsigned char *aes,int size){
    unsigned short i;

    for (i=0;i<size;i++){
        aes[i] = getRandom(0,255);
    }
}

/* ====================================================================== */
/* GENERA LAS 15 ROUND KEYS DE 16 BYTES A PARTIR DE LOS 32 BYTES aes[]   */
/* ====================================================================== */
void genKeyRound(unsigned char *aes, int size, roundKey keys[15]) {
    unsigned char temp[4];
    unsigned char w[60][4];  /* 60 palabras de 4 bytes = 240 bytes */
    int i;

    if (size != 32) return; /* AES-256 */

    /* ---- 1) w[0..7] vienen directo de la clave ---- */
    for (i = 0; i < 8; i++) {
        w[i][0] = aes[i*4 + 0];
        w[i][1] = aes[i*4 + 1];
        w[i][2] = aes[i*4 + 2];
        w[i][3] = aes[i*4 + 3];
    }

    /* ---- 2) Generar w[8..59] ---- */
    for (i = 8; i < 60; i++) {

        temp[0] = w[i-1][0];
        temp[1] = w[i-1][1];
        temp[2] = w[i-1][2];
        temp[3] = w[i-1][3];

        if (i % 8 == 0) {
            RotWord(temp);
            SubWord(temp);
            temp[0] ^= rcon[i/8];
        }
        else if (i % 8 == 4) {
            SubWord(temp);
        }

        w[i][0] = w[i-8][0] ^ temp[0];
        w[i][1] = w[i-8][1] ^ temp[1];
        w[i][2] = w[i-8][2] ^ temp[2];
        w[i][3] = w[i-8][3] ^ temp[3];
    }

    /* ---- 3) Armar las 15 round keys (cada 4 palabras = 16 bytes) ---- */
    for (i = 0; i < 15; i++) {
        int base = i * 4; /* w[base]..w[base+3] */
        memcpy(&keys[i].roundKey[0], w[base+0], 4);
        memcpy(&keys[i].roundKey[4], w[base+1], 4);
        memcpy(&keys[i].roundKey[8], w[base+2], 4);
        memcpy(&keys[i].roundKey[12], w[base+3], 4);
    }
}



/* aes algorithms */
/* Usa la misma sbox[256] ya definida arriba en aes.c */
/* Aplica la S-box a todos los bytes del estado */
void SubBytes(unsigned char state[16]) {
    int i;
    for (i = 0; i < 16; i++) {
        state[i] = sbox[state[i]];
    }
}

/* shift rows */
void ShiftRows(unsigned char state[16]) {
    unsigned char tmp;

    /* Fila 1: [1,5,9,13] -> rota 1 a la izquierda */
    tmp        = state[1];
    state[1]   = state[5];
    state[5]   = state[9];
    state[9]   = state[13];
    state[13]  = tmp;

    /* Fila 2: [2,6,10,14] -> rota 2 a la izquierda */
    tmp        = state[2];
    state[2]   = state[10];
    state[10]  = tmp;
    tmp        = state[6];
    state[6]   = state[14];
    state[14]  = tmp;

    /* Fila 3: [3,7,11,15] -> rota 3 a la izquierda (o 1 a la derecha) */
    tmp        = state[15];
    state[15]  = state[11];
    state[11]  = state[7];
    state[7]   = state[3];
    state[3]   = tmp;
}


/* mix colum and helpers */
/* multiplica por 2 en GF(2^8) con polinomio AES x^8 + x^4 + x^3 + x + 1 (0x1B) */
static unsigned char xtime(unsigned char x) {
    unsigned char res = (unsigned char)(x << 1);
    if (x & 0x80) {         /* si el bit mas alto estaba en 1, reducimos */
        res ^= 0x1B;
    }
    return res;
}

/* multiplicar por 3 en GF(2^8) es: 3*x = 2*x XOR x */
static unsigned char mul3(unsigned char x) {
    return (unsigned char)(xtime(x) ^ x);
}

void MixColumns(unsigned char state[16]) {
    int c;
    for (c = 0; c < 4; c++) {
        int base = 4 * c; /* indice de la columna: 0,4,8,12 */

        unsigned char s0 = state[base + 0];
        unsigned char s1 = state[base + 1];
        unsigned char s2 = state[base + 2];
        unsigned char s3 = state[base + 3];

        unsigned char r0 = (unsigned char)(xtime(s0) ^ mul3(s1) ^ s2 ^ s3);
        unsigned char r1 = (unsigned char)(s0 ^ xtime(s1) ^ mul3(s2) ^ s3);
        unsigned char r2 = (unsigned char)(s0 ^ s1 ^ xtime(s2) ^ mul3(s3));
        unsigned char r3 = (unsigned char)(mul3(s0) ^ s1 ^ s2 ^ xtime(s3));

        state[base + 0] = r0;
        state[base + 1] = r1;
        state[base + 2] = r2;
        state[base + 3] = r3;
    }
}


/* add round key */
void AddRoundKey(unsigned char state[16], const unsigned char rk[16]) {
    int i;
    for (i = 0; i < 16; i++) {
        state[i] ^= rk[i];
    }
}


/* encripta un bloque de 16 bytes usando AES-256 y las 15 round keys */
void Aes256EncryptBlock(unsigned char state[16], roundKey keys[15]) {
    int r;

    /* Ronda inicial: AddRoundKey con RK0 */
    AddRoundKey(state, keys[0].roundKey);

    /* Rondas 1 a 13: SubBytes, ShiftRows, MixColumns, AddRoundKey */
    for (r = 1; r <= 13; r++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, keys[r].roundKey);
    }

    /* Ronda 14 (ultima): sin MixColumns */
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, keys[14].roundKey);
}
