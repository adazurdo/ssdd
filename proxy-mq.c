//
// Created by adazu on 03/03/2026.
//
#include "claves.h"
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "proxy-mq.h"


int enviar(Peticion peticion) {
    char nom_cola[256];

    struct mq_attr attr;
    attr.mq_msgsize = sizeof(Peticion);
    attr.mq_curmsgs = 10;
    sprintf(nom_cola, "/CLIENTE_%d", getpid());
    strcpy(peticion.q_nom, nom_cola);

    mqd_t cliente = mq_open(nom_cola, O_CREAT | O_RDONLY, 0700, &attr);
    if (cliente == -1) {printf("Error al abrir cola"); return -1;}

    mqd_t servidor;
    servidor = mq_open("/SERVIDOR_MQ", O_WRONLY);
    if (servidor == -1) {printf("Error al abrir servidor"); return -1;}

    if (mq_send(servidor, (char*)&peticion, sizeof(Peticion), 0) == -1) {
        printf("Error al enviar peticion");
        mq_close(servidor);
        mq_close(cliente);
        mq_unlink(nom_cola);
        return -1;
    }

    Peticion respuesta;
    if (mq_receive(cliente, (char*)&respuesta, sizeof(Peticion), 0) == -1) {
        printf("Error al recibir respuesta");
        mq_close(servidor);
        mq_close(cliente);
        mq_unlink(nom_cola);
        return -1;
    }
    mq_close(servidor);
    mq_close(cliente);
    mq_unlink(nom_cola);

    return respuesta.op;
}

///////

int destroy() {
    Peticion peticion = {0};


    peticion.op = 0;

    return enviar(peticion);
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    Peticion peticion = {0};

    peticion.op = 1;
    strcpy(peticion.key, key);
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(peticion.V_value2, V_value2, sizeof(float) * N_value2);
    peticion.value3 = value3;

    return enviar(peticion);
}
int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    Peticion peticion = {0};

    peticion.op = 2;
    strcpy(peticion.key, key);

    if (enviar(peticion) == 0) {
        strcpy(value1, peticion.value1);
        *N_value2 = peticion.N_value2;
        for (int i = 0; i < peticion.N_value2; i++) {
            V_value2[i] = peticion.V_value2[i];
        }
        *value3 = peticion.value3;
    }
    return enviar(peticion);

}
int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    Peticion peticion = {0};

    peticion.op = 3;
    strcpy(peticion.key, key);
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(peticion.V_value2, V_value2, sizeof(float) * N_value2);
    peticion.value3 = value3;

    return enviar(peticion);


}
int delete_key(char *key) {
    Peticion peticion = {0};
    peticion.op = 4;
    strcpy(peticion.key, key);
    return enviar(peticion);

}
int exist(char *key) {
    Peticion peticion = {0};
    peticion.op = 5;
    strcpy(peticion.key, key);
    return enviar(peticion);

}
