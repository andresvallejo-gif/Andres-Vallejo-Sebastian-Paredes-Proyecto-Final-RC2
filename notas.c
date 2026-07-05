#include "notas.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Nota registros[MAX_REGISTROS];
static int registroCount = 0;

static void trimTrailingWhitespace(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' || str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[len - 1] = '\0';
        len--;
    }
}

void safeReadLine(char *buffer, int size) {
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
static float leerPonderacion(const char *prompt) {
    char buffer[32];
    float valor;

    while (1) {
        printf("%s", prompt);
        safeReadLine(buffer, sizeof(buffer));

        if (sscanf(buffer, "%f", &valor) == 1 && valor >= 0.0f && valor <= 100.0f) {
            return valor;
        }

        printf("Entrada invalida. Ingrese una ponderacion entre 0 y 100.\n");
    }
}

void registrarMateria(void) {
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
    nueva.ponderacionP1 = leerPonderacion("Ponderacion P1: ");
    nueva.ponderacionP2 = leerPonderacion("Ponderacion P2: ");
    nueva.ponderacionP3 = leerPonderacion("Ponderacion P3: ");

float suma = nueva.ponderacionP1 +
             nueva.ponderacionP2 +
             nueva.ponderacionP3;

if (suma < 99.99f || suma > 100.01f) {
    printf("Error: las ponderaciones deben sumar 100.\n");
    return;
}
    registros[registroCount++] = nueva;
    printf("Registro guardado correctamente.\n");
}

void actualizarRegistro(void) {
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

void eliminarRegistro(void) {
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
    return (nota->notaP1 * nota->ponderacionP1 +
            nota->notaP2 * nota->ponderacionP2 +
            nota->notaP3 * nota->ponderacionP3) / 100.0f;
}

void listarMaterias(void) {
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
void promedioPorEstudiante(void) {
    char codigoEstudiante[MAX_CODIGO];
    float suma = 0.0f;
    int contador = 0;

    printf("Codigo de estudiante: ");
    safeReadLine(codigoEstudiante, sizeof(codigoEstudiante));

    for (int i = 0; i < registroCount; i++) {
        if (strcmp(registros[i].codigoEstudiante, codigoEstudiante) == 0) {
            suma += calcularNotaFinal(&registros[i]);
            contador++;
        }
    }

    if (contador == 0) {
        printf("No se encontraron materias para ese estudiante.\n");
    } else {
        printf("Promedio del estudiante: %.2f\n", suma / contador);
    }
}

void promedioPorMateria(void) {
    char codigoMateria[MAX_CODIGO];
    float suma = 0.0f;
    int contador = 0;

    printf("Codigo de materia: ");
    safeReadLine(codigoMateria, sizeof(codigoMateria));

    for (int i = 0; i < registroCount; i++) {
        if (strcmp(registros[i].codigoMateria, codigoMateria) == 0) {
            suma += calcularNotaFinal(&registros[i]);
            contador++;
        }
    }

    if (contador == 0) {
        printf("No se encontraron estudiantes en esa materia.\n");
    } else {
        printf("Promedio de la materia: %.2f\n", suma / contador);
    }
}

void aprobadosReprobadosPorMateria(void) {
    char codigoMateria[MAX_CODIGO];
    char buffer[32];
    float umbral;
    int aprobados = 0;
    int reprobados = 0;

    printf("Codigo de materia: ");
    safeReadLine(codigoMateria, sizeof(codigoMateria));

    printf("Umbral de aprobacion: ");
    safeReadLine(buffer, sizeof(buffer));

    if (sscanf(buffer, "%f", &umbral) != 1) {
        printf("Umbral invalido.\n");
        return;
    }

    for (int i = 0; i < registroCount; i++) {
        if (strcmp(registros[i].codigoMateria, codigoMateria) == 0) {
            if (calcularNotaFinal(&registros[i]) >= umbral) {
                aprobados++;
            } else {
                reprobados++;
            }
        }
    }

    printf("Aprobados: %d\n", aprobados);
    printf("Reprobados: %d\n", reprobados);
}
void mostrarMenu(void) {
    printf("\n--- Menu principal ---\n");
    printf("1. Registrar materia\n");
    printf("2. Listar materias\n");
    printf("3. Actualizar registro\n");
    printf("4. Eliminar registro\n");
    printf("5. Guardar cambios en archivo\n");
    printf("6. Promedio por estudiante\n");
    printf("7. Promedio por materia\n");
    printf("8. Aprobados/Reprobados por materia\n");

    printf("0. Salir\n");
    printf("Opcion: ");
}

void cargarArchivo(void) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "r");
    if (archivo == NULL) {
        printf("No existe archivo previo. Se iniciara con registros vacios.\n");
        return;
    }

    char linea[512];
    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        fclose(archivo);
        return;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        trimTrailingWhitespace(linea);
        if (linea[0] == '\0') {
            continue;
        }

        char *token = strtok(linea, ";");
        if (token == NULL) {
            continue;
        }

        Nota nota;
        strncpy(nota.codigoMateria, token, sizeof(nota.codigoMateria) - 1);
        nota.codigoMateria[sizeof(nota.codigoMateria) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        strncpy(nota.nombreMateria, token, sizeof(nota.nombreMateria) - 1);
        nota.nombreMateria[sizeof(nota.nombreMateria) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        strncpy(nota.carrera, token, sizeof(nota.carrera) - 1);
        nota.carrera[sizeof(nota.carrera) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        strncpy(nota.codigoEstudiante, token, sizeof(nota.codigoEstudiante) - 1);
        nota.codigoEstudiante[sizeof(nota.codigoEstudiante) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        strncpy(nota.nombreEstudiante, token, sizeof(nota.nombreEstudiante) - 1);
        nota.nombreEstudiante[sizeof(nota.nombreEstudiante) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        nota.notaP1 = strtof(token, NULL);

        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        nota.notaP2 = strtof(token, NULL);

        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        nota.notaP3 = strtof(token, NULL);
        token = strtok(NULL, ";");
nota.ponderacionP1 = token != NULL ? strtof(token, NULL) : 30.0f;

token = strtok(NULL, ";");
nota.ponderacionP2 = token != NULL ? strtof(token, NULL) : 30.0f;

token = strtok(NULL, ";");
nota.ponderacionP3 = token != NULL ? strtof(token, NULL) : 40.0f;

        if (registroCount < MAX_REGISTROS && !existeRegistro(nota.codigoMateria, nota.codigoEstudiante)) {
            registros[registroCount++] = nota;
        }
    }

    fclose(archivo);
    printf("Datos cargados correctamente desde %s.\n", ARCHIVO_DATOS);
}

int guardarArchivo(void) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "w");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo para guardar.\n");
        return 0;
    }

    fprintf(archivo, "codigo_materia;nombre_materia;carrera;codigo_estudiante;nombre_estudiante;nota_p1;nota_p2;nota_p3;ponderacion_p1;ponderacion_p2;ponderacion_p3\n");   
    for (int i = 0; i < registroCount; i++) {
        const Nota *n = &registros[i];
        fprintf(archivo, "%s;%s;%s;%s;%s;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f\n",
                n->codigoMateria,
                n->nombreMateria,
                n->carrera,
                n->codigoEstudiante,
                n->nombreEstudiante,
                n->notaP1,
                n->notaP2,
                n->notaP3,
                n->ponderacionP1,
                n->ponderacionP2,
                n->ponderacionP3);
                
    }

    fclose(archivo);
    printf("Archivo %s actualizado correctamente.\n", ARCHIVO_DATOS);
    return 1;
}
