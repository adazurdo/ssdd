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
int clave;

typedef struct Tupla {
    char key[256];
    char value1[256];
    int N_value2;
    float value2[32];
    struct Paquete value3;
};

int destroy(void) {

    if (remove(FICHERO) == -1) {
        return -1;
    }

    return 0;
}

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
        fclose(f);
    } // si no existe, se crea
    f = fopen(FICHERO, "ab");
    if (f == NULL) {return -1;}
    //rellenamos los campos de la tupla
    strcpy(t.key, key);
    strcpy(t.value1, value1);
    t.N_value2 = N_value2;
    for (int i = 0; i < N_value2; i++) {t.value2[i] = V_value2[i]; }
    //insertamos
    fwrite(&t, sizeof(struct Tupla), 1, f);
    //cerramos fichero
    fclose(f);
    return 0;
}

////////////////
int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    //DEVUELVE LOS VALORES ASOCIADO SA UNA CLAVE
    FILE *f;
    struct Tupla t;
    f = fopen(FICHERO, "rb");
    if (f != NULL) { //mientars q no sea null leo en trozos del tmñ de la tupla
        while (fread(&t, sizeof(struct Tupla), 1, f) == 1) {//mitrs q no de error la lect
            if (strcmp(key, t.key) == 0) { //si he encintrado la clave
               strcpy(value1, t.value1);
                *N_value2 = t.N_value2;
                for (int i = 0; i < t.N_value2; i++){
                    V_value2[i] = t.value2[i];}
                *value3 = t.value3;

                fclose(f);
                return 0; // si
            }
        }
        fclose(f);
        return -1;
    }

}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    FILE *f;
    struct Tupla t;

    f = fopen(FICHERO, "rb+");

    if (f != NULL) { //mientars q no sea null leo en trozos del tmñ de la tupla
        while (fread(&t, sizeof(struct Tupla), 1, f) == 1) {//mitrs q no de error la lect
                //comparamos con nuestro valor de tupla que nos han pasasdo
            if (strcmp(key, t.key) == 0) { //devuelve 0 cuando son iguales
               //si encunetro la clave
                strcpy(t.value1, value1);
                t.N_value2 = N_value2;
                for (int i = 0; i < t.N_value2; i++){
                    t.value2[i] = V_value2[i];}
                t.value3 = value3;
                //vuelvo a donde mepieza la tupla
                fseek(f, -sizeof(struct Tupla), SEEK_CUR);
                fwrite(&t, sizeof(struct Tupla), 1, f);

                fclose(f);
                return 0;
            }
        }
        fclose(f);
        return -1;
    }
}

int delete_key(char *key) {
    FILE *f;
    FILE *aux;
    struct Tupla t;
    int encontrado = 0;

    f = fopen(FICHERO, "rb");
    aux = fopen("temp", "wb");
    if (f != NULL) {
        while (fread(&t, sizeof(struct Tupla), 1, f) == 1) {
            if (strcmp(key, t.key) == 0) {
                encontrado = 1;
            }
            fwrite(&t, sizeof(struct Tupla), 1, aux);
        }
        fclose(f);
        fclose(aux);
        if (encontrado == 0){
            //si la clave no existe no necesito el temp porq no tengo que borrar nada
            remove("temp");
            return -1;
        }
        remove(FICHERO);
        rename("temp", FICHERO);
        return 0;

    }
}

int exist(char *key) {
    FILE *f;
    struct Tupla t;

    f = fopen(FICHERO, "rb");

    if (f != NULL) { //mientars q no sea null leo en trozos del tmñ de la tupla
        while (fread(&t, sizeof(struct Tupla), 1, f) == 1) {//mitrs q no de error la lect
            if (strcmp(key, t.key) == 0) { //si he encintrado la clave
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}
