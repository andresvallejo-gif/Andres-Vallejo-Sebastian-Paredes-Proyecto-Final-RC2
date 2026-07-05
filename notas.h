#ifndef NOTAS_H
#define NOTAS_H

#include <stdio.h>

#define MAX_REGISTROS 100
#define MAX_CODIGO 16
#define MAX_NOMBRE 51
#define ARCHIVO_DATOS "notas.csv"

typedef struct {
    char codigoMateria[MAX_CODIGO];
    char nombreMateria[MAX_NOMBRE];
    char carrera[MAX_NOMBRE];
    char codigoEstudiante[MAX_CODIGO];
    char nombreEstudiante[MAX_NOMBRE];
    float notaP1;
    float notaP2;
    float notaP3;
    float ponderacionP1;
    float ponderacionP2;
    float ponderacionP3;
    
} Nota;

void safeReadLine(char *buffer, int size);
void cargarArchivo(void);
int guardarArchivo(void);
void registrarMateria(void);
void actualizarRegistro(void);
void eliminarRegistro(void);
void listarMaterias(void);
void mostrarMenu(void);
void promedioPorEstudiante(void);
void promedioPorMateria(void);
void aprobadosReprobadosPorMateria(void);


#endif // NOTAS_H
