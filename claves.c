//
// Created by adazu on 23/02/2026.
//

//El archivo claves.c implementa un servicio para almacenar tuplas de la forma <key-value1-value2-value3>

//en C no existen tuplas tal cual, se usa un STRUCT

//puedo elegir la estructura de datos que yo desee. elijo FICHERO BINARIO

//voy a hacer un unico dorectorio que alamcene todas las tuplas

#include "claves.h"

#include <stdio.h>
#include <string.h>
#define FICHERO "data.bin"
int inicializado;
int clave;

typedef struct Tupla {
    char key[256];
    char value1[256];
    int N_value2;
    float value2[32];
    struct Paquete value3;
};

int destroy(void) {
    inicializado = 0;

    if (remove(FICHERO) == -1) {
        return -1;
    }

    inicializado = 1;
    return 0;
}


int f;
///////////////////////////
int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
                    //Este servicio inserta el elemento <key, value1, value2, value3>.
                    //Se considera error, intentar insertar una clave que ya existe previamente
                    //o que el valor N_value2 esté fuera de rango

    if (N_value2 < 1 || N_value2 > 32) { return -1;}

    FILE *f;
    struct Tupla t;

    //primero debemos asegurarnos de que no existe la clave
    f = fopen(FICHERO, "rb");
    if (f != NULL) {
        //leemos una tupla a la vez
        while (fread(&t, sizeof(struct Tupla), 1, f) == 1) {
            //comparamos con nuestro valor de tupla que nos han pasasdo
            if (strcmp(key, t.key) == 0) { //devuelve 0 cuando son iguales
                fclose(f);  //como son iguales, es decir, ya existe la tupla, cierro y mando error
                return -1;
            }
        }
    } // si no existe, se crea

    f = fopen(FICHERO, "ab");
    if (f == NULL) {return -1;}



    fclose(f);
    return 0;
}