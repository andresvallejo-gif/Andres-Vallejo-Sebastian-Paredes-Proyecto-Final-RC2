#include <stdio.h>
#include <string.h>
#include "notas.h"

int main(void) {
    char opcion[8];

    cargarArchivo();
    while (1) {
        mostrarMenu();
        safeReadLine(opcion, sizeof(opcion));

        if (strcmp(opcion, "1") == 0) {
            registrarMateria();
        } else if (strcmp(opcion, "2") == 0) {
            listarMaterias();
        } else if (strcmp(opcion, "3") == 0) {
            actualizarRegistro();
        } else if (strcmp(opcion, "4") == 0) {
            eliminarRegistro();
        } else if (strcmp(opcion, "5") == 0) {
            if (guardarArchivo()) {
                printf("Cambios guardados en %s.\n", ARCHIVO_DATOS);
            }
        } else if (strcmp(opcion, "0") == 0) {
            if (guardarArchivo()) {
                printf("Cambios guardados en %s.\n", ARCHIVO_DATOS);
            }
            printf("Saliendo...\n");
            break;
        } else {
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    }

    return 0;
}
