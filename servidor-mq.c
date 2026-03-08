//
// Created by adazu on 03/03/2026.
//

#include "servidor-mq.h"
#include "claves.h"
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

//ya no necesito esperar, mutex ni cond para bloquear al hilo principal
//int esperar = 1;
//pthread_mutex_t mutex;
//pthread_cond_t cond;
mqd_t q_servidor;

//función del hilo: ahora con la firma estándar de pthread
void *gestionar_mensaje(void *peticion_main) {
    mqd_t q_cliente;
    int operacion;
    Peticion peticion_loc;

    //necesito loc y main porq al final es una variable que se acaba sobreescribiendo
    peticion_loc = (*(Peticion*)peticion_main);

    //ya no bloqueo con mutex ni uso cond aquí, no hace falta para este diseño
    //esperar = 0;
    //pthread_cond_init(&cond, NULL);

    //ahora tengo que ejecutar lo que me ha pedido el cliente
    //como se pueden llamar a muchas funciones hago uso de switch
    switch (peticion_loc.op) {

        case 0: operacion = destroy();
            break;
        case 1: operacion = set_value(peticion_loc.key, peticion_loc.value1,
            peticion_loc.N_value2, peticion_loc.V_value2, peticion_loc.value3);
            break;
        case 2: operacion = get_value(peticion_loc.key, peticion_loc.value1,
            &peticion_loc.N_value2, peticion_loc.V_value2, &peticion_loc.value3);
            break;
        case 3: operacion = modify_value(peticion_loc.key, peticion_loc.value1,
            peticion_loc.N_value2, peticion_loc.V_value2, peticion_loc.value3);
            break;
        case 4: operacion = delete_key(peticion_loc.key);
            break;
        case 5: operacion = exist(peticion_loc.key);
            break;
        default: operacion = -1; //caso de error
            break;
    }

    q_cliente = mq_open(peticion_loc.q_nom, O_WRONLY);
    if (q_cliente == -1) {
        printf("Error al crear el cliente\n");
        //no cierro ni unlinkeo la cola del servidor aquí, para no tumbar el servidor por un cliente
        pthread_exit(NULL);
    }

    peticion_loc.op = operacion;
    if (mq_send(q_cliente, (char*)&peticion_loc, sizeof(peticion_loc), 0) == -1) {
        printf("Error al enviar el cliente\n");
        mq_close(q_cliente);
        pthread_exit(NULL);
    }

    mq_close(q_cliente);
    pthread_exit(NULL); //equivalente a return NULL;
}

int main(int argc, char *argv[]) {

    struct mq_attr attr;
    Peticion peticion_main;


    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(peticion_main);

    //inicializo lo necesario para los threads
    pthread_attr_t att;
    pthread_attr_init(&att);

    //ya no necesito mutex ni cond para esperar al hilo
    //pthread_mutex_init(&mutex, NULL);
    //pthread_cond_init(&cond, NULL);

    pthread_t hilo;

    q_servidor = mq_open("/SERVIDOR_MQ", O_RDONLY | O_CREAT, 0700, &attr);
    if (q_servidor == -1) {
        printf("Error al crear el servidor\n");
        return 1;
    }

    while (1) {
        if (mq_receive(q_servidor, (char*)&peticion_main, sizeof(peticion_main), 0) == -1){
            printf("Error al recibir el peticion\n");
            return -1;
        }
        //como vamos a trabajar de manera concurrente debemos de lanzar threads
        if (pthread_create(&hilo, &att, (void*)gestionar_mensaje, (void*)&peticion_main) == 0) {
            //ya no tengo que esperar al thread, lo dejo trabajar en paralelo
            pthread_detach(hilo); //para no tener que hacer join
        } else {
            printf("Error al crear el hilo\n");
        }
    }
}
