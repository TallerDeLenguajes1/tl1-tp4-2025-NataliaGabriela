#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 100

typedef struct {
    int TareaID;      // ID generado automáticamente (comienza en 1000)
    char *Descripcion; // Descripción de la tarea
    int Duracion;     // Duración entre 10 y 100
} Tarea;

Tarea **cargartareas(int *cant_tareas, Tarea **p);
void transferir_tareas(Tarea **pendientes, Tarea **realizadas, int *pendientes_count, int *realizadas_count);
void listar_tareas(Tarea **pendientes, Tarea **realizadas, int pendientes_count, int realizadas_count);
void consultar_tareas(Tarea **pendientes, Tarea **realizadas, int pendientes_count, int realizadas_count);
void liberar_memoria(Tarea **p, int cant_tareas);

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    srand(time(NULL));
    Tarea **pendientes = NULL, **realizadas = NULL;
    int cant_tareas_pendientes = 0, cant_tareas_realizadas = 0;

    // Cargar tareas pendientes
    pendientes = cargartareas(&cant_tareas_pendientes, pendientes);

    int opcion;
    do {
        printf("\nMenú de opciones:\n");
        printf("1. Listar tareas pendientes y realizadas\n");
        printf("2. Consultar tareas por ID o palabra clave\n");
        printf("3. Transferir tareas pendientes a realizadas\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                listar_tareas(pendientes, realizadas, cant_tareas_pendientes, cant_tareas_realizadas);
                break;
            case 2:
                consultar_tareas(pendientes, realizadas, cant_tareas_pendientes, cant_tareas_realizadas);
                break;
            case 3:
                transferir_tareas(pendientes, realizadas, &cant_tareas_pendientes, &cant_tareas_realizadas);
                break;
            case 4:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }
    } while(opcion != 4);

    // Liberar memoria al final
    liberar_memoria(pendientes, cant_tareas_pendientes);
    liberar_memoria(realizadas, cant_tareas_realizadas);
    return 0;
}

Tarea **cargartareas(int *cant_tareas, Tarea **p) {
    int i, tam_des;
    char descripcion[MAX];
    *cant_tareas = 0;
    int continuar = 1;
    
    // Iniciar un ciclo para ingresar tareas
    while (continuar) {
        // Reasignamos memoria para la nueva tarea
        p = (Tarea **)realloc(p, (*cant_tareas + 1) * sizeof(Tarea *));
        if (p == NULL) {
            printf("No se pudo asignar memoria para la lista de tareas.\n");
            exit(1);
        }

        // Asignamos memoria para la tarea
        p[*cant_tareas] = (Tarea *)malloc(sizeof(Tarea));
        if (p[*cant_tareas] == NULL) {
            printf("No se pudo asignar memoria para la tarea.\n");
            exit(1);
        }

        // Asignar ID autoincremental
        p[*cant_tareas]->TareaID = 1000 + *cant_tareas;

        // Ingresar la descripción de la tarea
        printf("Ingrese la descripción de la tarea %d: ", p[*cant_tareas]->TareaID);
        scanf(" %[^\n]", descripcion);  // Leer hasta el salto de línea
        tam_des = strlen(descripcion) + 1;  // Reservar espacio para '\0'
        
        // Asignar memoria para la descripción de la tarea
        p[*cant_tareas]->Descripcion = (char *)malloc(tam_des * sizeof(char));
        if (p[*cant_tareas]->Descripcion == NULL) {
            printf("No se pudo asignar memoria para la descripción de la tarea.\n");
            exit(1);
        }

        // Copiar la descripción ingresada
        strcpy(p[*cant_tareas]->Descripcion, descripcion);

        // Ingresar la duración de la tarea
        printf("Ingrese la duración de la tarea (10-100): ");
        scanf("%d", &p[*cant_tareas]->Duracion);
        while (getchar() != '\n'); 
        if (p[*cant_tareas]->Duracion < 10 || p[*cant_tareas]->Duracion > 100) {
            printf("Duración no válida. Debe estar entre 10 y 100.\n");
            free(p[*cant_tareas]->Descripcion);  // Liberar memoria en caso de error
            free(p[*cant_tareas]);  // Liberar memoria en caso de error
            continue;  // Continuar pidiendo la misma tarea
        }

        // Incrementar el contador de tareas
        (*cant_tareas)++;

        // Preguntar si desea ingresar otra tarea
        printf("¿Desea ingresar otra tarea? (1: sí, 0: no): ");
        scanf("%d", &continuar);
        while (getchar() != '\n'); 
    }

    return p;
}


void transferir_tareas(Tarea **pendientes, Tarea **realizadas, int *pendientes_count, int *realizadas_count) {
    int id, i, found = 0;

    printf("Ingrese el ID de la tarea a transferir a realizadas: ");
    scanf("%d", &id);

    for (i = 0; i < *pendientes_count; i++) {
        if (pendientes[i] != NULL && pendientes[i]->TareaID == id) {
            // Asignar tarea a las realizadas
            realizadas = (Tarea **)realloc(realizadas, (*realizadas_count + 1) * sizeof(Tarea *));
            if (realizadas == NULL) {
                printf("No se pudo asignar memoria para las tareas realizadas.\n");
                exit(1);
            }

            realizadas[*realizadas_count] = pendientes[i];
            pendientes[i] = NULL;  // Marcar tarea como movida

            (*realizadas_count)++;
            (*pendientes_count)--;

            printf("Tarea transferida a realizadas con éxito.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No se encontró la tarea con el ID %d.\n", id);
    }
}

void listar_tareas(Tarea **pendientes, Tarea **realizadas, int pendientes_count, int realizadas_count) {
    printf("\nTareas Pendientes:\n");
    for (int i = 0; i < pendientes_count; i++) {
        if (pendientes[i] != NULL) {
            printf("ID: %d - Descripción: %s - Duración: %d\n", pendientes[i]->TareaID, pendientes[i]->Descripcion, pendientes[i]->Duracion);
        }
    }

    printf("\nTareas Realizadas:\n");
    for (int i = 0; i < realizadas_count; i++) {
        printf("ID: %d - Descripción: %s - Duración: %d\n", realizadas[i]->TareaID, realizadas[i]->Descripcion, realizadas[i]->Duracion);
    }
}

void consultar_tareas(Tarea **pendientes, Tarea **realizadas, int pendientes_count, int realizadas_count) {
    char keyword[MAX];
    printf("Ingrese el ID o palabra clave para buscar: ");
    scanf(" %[^\n]", keyword);

    int found = 0;
    // Buscar en tareas pendientes
    for (int i = 0; i < pendientes_count; i++) {
        if (pendientes[i] != NULL && (strstr(pendientes[i]->Descripcion, keyword) != NULL || pendientes[i]->TareaID == atoi(keyword))) {
            printf("Tarea Pendiente - ID: %d - Descripción: %s - Duración: %d\n", pendientes[i]->TareaID, pendientes[i]->Descripcion, pendientes[i]->Duracion);
            found = 1;
        }
    }

    // Buscar en tareas realizadas
    for (int i = 0; i < realizadas_count; i++) {
        if (strstr(realizadas[i]->Descripcion, keyword) != NULL || realizadas[i]->TareaID == atoi(keyword)) {
            printf("Tarea Realizada - ID: %d - Descripción: %s - Duración: %d\n", realizadas[i]->TareaID, realizadas[i]->Descripcion, realizadas[i]->Duracion);
            found = 1;
        }
    }

    if (!found) {
        printf("No se encontró ninguna tarea con el ID o palabra clave proporcionada.\n");
    }
}

void liberar_memoria(Tarea **p, int cant_tareas) {
    for (int i = 0; i < cant_tareas; i++) {
        if (p[i] != NULL) {
            free(p[i]->Descripcion);
            free(p[i]);
        }
    }
    free(p);
}
