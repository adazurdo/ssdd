#include <stdio.h>
#include "claves.h"

int main() {

    printf("=== INICIO DEL PROGRAMA ===\n");

    // INSERTAMOS PRIMERA TUPLA
    printf("\n[1] Insertando primera tupla...\n");
    char *key = "clave 1";
    char *v1 = "ejemplo de valor 1";
    float v2[] = {3, 5, 45};
    struct Paquete v3;
    v3.x = 10;
    v3.y = 5;
    v3.z = 3;

    int err = set_value(key, v1, 3, v2, v3);
    if (err == -1) {
        printf("❌ Error al insertar la primera tupla\n");
    } else {
        printf("✔ Primera tupla insertada correctamente\n");
    }

    // INSERTAMOS SEGUNDA TUPLA
    printf("\n[2] Insertando segunda tupla...\n");
    char *key2 = "clave 2";
    char *v1_2 = "ejemplo de valor 2";
    float v2_2[] = {8, 7, 45};
    struct Paquete v3_2;
    v3_2.x = 1;
    v3_2.y = 2;
    v3_2.z = 3;

    int err2 = set_value(key2, v1_2, 3, v2_2, v3_2);
    if (err2 == -1) {
        printf("❌ Error al insertar la segunda tupla\n");
    } else {
        printf("✔ Segunda tupla insertada correctamente\n");
    }

    // GET
    printf("\n[3] Probando GET sobre clave 2...\n");
    char buffer_v1[256];
    int N_value2_get;
    float buffer_v2[32];
    struct Paquete paquete_get;

    int get = get_value(key2, buffer_v1, &N_value2_get, buffer_v2, &paquete_get);
    if (get == -1) {
        printf("❌ Error al hacer GET\n");
    } else {
        printf("✔ GET correcto\n");
        printf("  value1 = %s\n", buffer_v1);
        printf("  N_value2 = %d\n", N_value2_get);
        for (int i = 0; i < N_value2_get; i++) {
            printf("  value2[%d] = %.2f\n", i, buffer_v2[i]);
        }
        printf("  value3 = (%d, %d, %d)\n", paquete_get.x, paquete_get.y, paquete_get.z);
    }

    // MODIFY
    printf("\n[4] Probando MODIFY sobre clave 1...\n");
    char *v1_mod = "ejemplo de valor 1";
    float v2_mod[] = {3, 5, 45};
    struct Paquete v3_mod;
    v3_mod.x = 10;
    v3_mod.y = 5;
    v3_mod.z = 3;

    int mod = modify_value(key, v1_mod, 3, v2_mod, v3_mod);
    if (mod == -1) {
        printf("❌ Error al modificar la tupla\n");
    } else {
        printf("✔ Tupla modificada correctamente\n");

        // MOSTRAR LO QUE HA QUEDADO TRAS EL MODIFY
        printf("  → Comprobando valores tras MODIFY...\n");

        char buf_mod_v1[256];
        int buf_mod_n;
        float buf_mod_v2[32];
        struct Paquete buf_mod_v3;

        int get_mod = get_value(key, buf_mod_v1, &buf_mod_n, buf_mod_v2, &buf_mod_v3);
        if (get_mod == -1) {
            printf("  ❌ Error al leer la tupla modificada\n");
        } else {
            printf("  ✔ Tupla modificada recuperada correctamente:\n");
            printf("    value1 = %s\n", buf_mod_v1);
            printf("    N_value2 = %d\n", buf_mod_n);
            for (int i = 0; i < buf_mod_n; i++) {
                printf("    value2[%d] = %.2f\n", i, buf_mod_v2[i]);
            }
            printf("    value3 = (%d, %d, %d)\n", buf_mod_v3.x, buf_mod_v3.y, buf_mod_v3.z);
        }
    }

    // DELETE
    printf("\n[5] Probando DELETE sobre clave 1...\n");
    int del = delete_key(key);
    if (del == -1) {
        printf("❌ Error al eliminar la tupla\n");
    } else {
        printf("✔ Tupla eliminada correctamente\n");
    }

    // EXIST
    printf("\n[6] Probando EXIST sobre clave 2...\n");
    int ex = exist(key2);
    if (ex == 1) {
        printf("✔ La tupla con clave 2 EXISTE\n");
    } else {
        printf("❌ La tupla con clave 2 NO existe\n");
    }

    // DESTROY
    printf("\n[7] Probando DESTROY...\n");
    int des = destroy();
    if (des == -1) {
        printf("❌ Error al destruir el fichero de datos\n");
    } else {
        printf("✔ Fichero destruido correctamente\n");
    }

    printf("\n=== FIN DEL PROGRAMA ===\n");

    return 0;
}
