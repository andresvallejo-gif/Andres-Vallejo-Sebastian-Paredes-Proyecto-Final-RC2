#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_REGISTROS 100
#define MAX_CODIGO 16
#define MAX_NOMBRE 51

typedef struct {
    char codigoMateria[MAX_CODIGO];
    char nombreMateria[MAX_NOMBRE];
    char carrera[MAX_NOMBRE];
    char codigoEstudiante[MAX_CODIGO];
    char nombreEstudiante[MAX_NOMBRE];
    float notaP1;
    float notaP2;
    float notaP3;
} Nota;

static Nota registros[MAX_REGISTROS];
static int registroCount = 0;

static void trimTrailingWhitespace(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' || str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[len - 1] = '\0';
        len--;
    }
}

static void safeReadLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        trimTrailingWhitespace(buffer);
    } else {
        buffer[0] = '\0';
        clearerr(stdin);
    }
}

static int isAlnumNoSpaces(const char *str) {
    if (str[0] == '\0') {
        return 0;
    }

    for (const char *p = str; *p; p++) {
        if (isspace((unsigned char)*p) || !isalnum((unsigned char)*p)) {
            return 0;
        }
    }

    return 1;
}

static int existeRegistro(const char *codigoMateria, const char *codigoEstudiante) {
    for (int i = 0; i < registroCount; i++) {
        if (strcmp(registros[i].codigoMateria, codigoMateria) == 0 &&
            strcmp(registros[i].codigoEstudiante, codigoEstudiante) == 0) {
            return 1;
        }
    }
    return 0;
}

static int buscarRegistroIndex(const char *codigoMateria, const char *codigoEstudiante) {
    for (int i = 0; i < registroCount; i++) {
        if (strcmp(registros[i].codigoMateria, codigoMateria) == 0 &&
            strcmp(registros[i].codigoEstudiante, codigoEstudiante) == 0) {
            return i;
        }
    }
    return -1;
}

static int confirmarAccion(const char *mensaje) {
    char respuesta[8];
    printf("%s (S/N): ", mensaje);
    safeReadLine(respuesta, sizeof(respuesta));
    return respuesta[0] == 'S' || respuesta[0] == 's';
}

static void actualizarCampoString(const char *prompt, char *dest, int size) {
    char buffer[MAX_NOMBRE];
    printf("%s (Enter para mantener: %s): ", prompt, dest);
    safeReadLine(buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(dest, buffer, size - 1);
        dest[size - 1] = '\0';
    }
}

static void leerNotaOpcional(const char *prompt, float *valorActual) {
    char buffer[32];
    float valor;
    while (1) {
        printf("%s (Enter para mantener %.2f): ", prompt, *valorActual);
        safeReadLine(buffer, sizeof(buffer));
        if (buffer[0] == '\0') {
            return;
        }
        if (sscanf(buffer, "%f", &valor) == 1 && valor >= 1.0f && valor <= 10.0f) {
            *valorActual = valor;
            return;
        }
        printf("Entrada invalida. Ingrese un numero entre 1 y 10 o deje en blanco.\n");
    }
}

static float leerNota(const char *prompt) {
    char buffer[32];
    float valor;
    while (1) {
        printf("%s", prompt);
        safeReadLine(buffer, sizeof(buffer));
        if (sscanf(buffer, "%f", &valor) == 1) {
            if (valor >= 1.0f && valor <= 10.0f) {
                return valor;
            }
        }
        printf("Entrada invalida. Ingrese un numero entre 1 y 10 con hasta 2 decimales.\n");
    }
}

static void registrarMateria(void) {
    if (registroCount >= MAX_REGISTROS) {
        printf("No se pueden registrar mas materias.\n");
        return;
    }

    Nota nueva;

    printf("Codigo de materia: ");
    safeReadLine(nueva.codigoMateria, sizeof(nueva.codigoMateria));
    if (!isAlnumNoSpaces(nueva.codigoMateria) || strlen(nueva.codigoMateria) >= MAX_CODIGO) {
        printf("Codigo de materia invalido. Debe ser alfanumerico sin espacios y maximo 15 caracteres.\n");
        return;
    }

    printf("Nombre de materia: ");
    safeReadLine(nueva.nombreMateria, sizeof(nueva.nombreMateria));
    if (nueva.nombreMateria[0] == '\0') {
        printf("El nombre de la materia no puede estar vacio.\n");
        return;
    }

    printf("Carrera: ");
    safeReadLine(nueva.carrera, sizeof(nueva.carrera));
    if (nueva.carrera[0] == '\0') {
        printf("La carrera no puede estar vacia.\n");
        return;
    }

    printf("Codigo de estudiante: ");
    safeReadLine(nueva.codigoEstudiante, sizeof(nueva.codigoEstudiante));
    if (!isAlnumNoSpaces(nueva.codigoEstudiante) || strlen(nueva.codigoEstudiante) >= MAX_CODIGO) {
        printf("Codigo de estudiante invalido. Debe ser alfanumerico sin espacios y maximo 15 caracteres.\n");
        return;
    }

    if (existeRegistro(nueva.codigoMateria, nueva.codigoEstudiante)) {
        printf("Ya existe un registro con este codigo de materia y codigo de estudiante.\n");
        return;
    }

    printf("Nombre de estudiante: ");
    safeReadLine(nueva.nombreEstudiante, sizeof(nueva.nombreEstudiante));
    if (nueva.nombreEstudiante[0] == '\0') {
        printf("El nombre del estudiante no puede estar vacio.\n");
        return;
    }

    nueva.notaP1 = leerNota("Nota P1: ");
    nueva.notaP2 = leerNota("Nota P2: ");
    nueva.notaP3 = leerNota("Nota P3: ");

    registros[registroCount++] = nueva;
    printf("Registro guardado correctamente.\n");
}

static void actualizarRegistro(void) {
    char codigoMateria[MAX_CODIGO];
    char codigoEstudiante[MAX_CODIGO];

    printf("Codigo de materia: ");
    safeReadLine(codigoMateria, sizeof(codigoMateria));
    printf("Codigo de estudiante: ");
    safeReadLine(codigoEstudiante, sizeof(codigoEstudiante));

    int indice = buscarRegistroIndex(codigoMateria, codigoEstudiante);
    if (indice < 0) {
        printf("Registro no encontrado.\n");
        return;
    }

    Nota *registro = &registros[indice];
    printf("Registro encontrado:\n");
    printf("Materia: %s | Estudiante: %s\n", registro->nombreMateria, registro->nombreEstudiante);

    actualizarCampoString("Nuevo nombre de materia", registro->nombreMateria, sizeof(registro->nombreMateria));
    actualizarCampoString("Nueva carrera", registro->carrera, sizeof(registro->carrera));
    actualizarCampoString("Nuevo nombre de estudiante", registro->nombreEstudiante, sizeof(registro->nombreEstudiante));

    leerNotaOpcional("Nueva nota P1", &registro->notaP1);
    leerNotaOpcional("Nueva nota P2", &registro->notaP2);
    leerNotaOpcional("Nueva nota P3", &registro->notaP3);

    printf("Registro actualizado correctamente.\n");
}

static void eliminarRegistro(void) {
    char codigoMateria[MAX_CODIGO];
    char codigoEstudiante[MAX_CODIGO];

    printf("Codigo de materia: ");
    safeReadLine(codigoMateria, sizeof(codigoMateria));
    printf("Codigo de estudiante: ");
    safeReadLine(codigoEstudiante, sizeof(codigoEstudiante));

    int indice = buscarRegistroIndex(codigoMateria, codigoEstudiante);
    if (indice < 0) {
        printf("Registro no encontrado.\n");
        return;
    }

    if (!confirmarAccion("Confirma eliminar este registro")) {
        printf("Eliminacion cancelada.\n");
        return;
    }

    for (int i = indice; i < registroCount - 1; i++) {
        registros[i] = registros[i + 1];
    }
    registroCount--;
    printf("Registro eliminado correctamente.\n");
}

static float calcularNotaFinal(const Nota *nota) {
    return (nota->notaP1 + nota->notaP2 + nota->notaP3) / 3.0f;
}

static void listarMaterias(void) {
    if (registroCount == 0) {
        printf("No hay registros para mostrar.\n");
        return;
    }

    printf("%-15s %-20s %-15s %-15s %-20s %6s %6s %6s %10s\n",
           "cod_mat", "nombre_mat", "carrera", "cod_est", "nombre_est", "P1", "P2", "P3", "final");
    for (int i = 0; i < registroCount; i++) {
        const Nota *n = &registros[i];
        printf("%-15s %-20s %-15s %-15s %-20s %6.2f %6.2f %6.2f %10.2f\n",
               n->codigoMateria,
               n->nombreMateria,
               n->carrera,
               n->codigoEstudiante,
               n->nombreEstudiante,
               n->notaP1,
               n->notaP2,
               n->notaP3,
               calcularNotaFinal(n));
    }
}

static void mostrarMenu(void) {
    printf("\n--- Menu principal ---\n");
    printf("1. Registrar materia\n");
    printf("2. Listar materias\n");
    printf("3. Actualizar registro\n");
    printf("4. Eliminar registro\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

int main(void) {
    char opcion[8];

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
        } else if (strcmp(opcion, "0") == 0) {
            printf("Saliendo...\n");
            break;
        } else {
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    }

    return 0;
}
