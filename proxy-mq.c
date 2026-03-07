//
// Created by adazu on 03/03/2026.
//

#include "proxy-mq.h"


int enviar(Peticion) {
    char nom_cola[256];

    struct mq_attr attr;
    attr.mq_msgsize = sizeof(Peticion);
    attr.mq_curmsgs = 10;


    mqd_t cliente;
    cliente = mq_open(nom_cola, O_CREAT|O_WRONLY, &attr, 0);
    if (cliente == -1) {printf("Error al abrir cola"); return -1;}

    mqd_t servidor;
    servidor = mq_open(nom_cola, O_WRONLY, &attr, 0);
    if (servidor == -1) {printf("Error al abrir servidor"); return -1;}

    if (mq_send(servidor, peticion, sizeof(Peticion), 0) == -1) {
        printf("Error al enviar peticion");
        mq_close(servidor);
        mq_close(cliente);
        mq_unlink(nom_cola);
        return -1;
    }
}



///////
int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    Peticion peticion;

}
int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    Peticion peticion;

}
int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    Peticion peticion;
}
int delete_key(char *key) {
    Peticion peticion;

}
int exist(char *key) {
    Peticion peticion;

}
