#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 100

// Estructura para las tareas
typedef struct
{
    int TareaID;       // ID generado automáticamente (comienza en 1000)
    char *Descripcion; // Descripción de la tarea
    int Duracion;      // Duración entre 10 y 100
} Tarea;

// Estructura para los nodos de la lista enlazada
typedef struct Nodo
{
    Tarea T;
    struct Nodo *Siguiente;
} Nodo;

// Declaración de funciones
Nodo *buscar_tarea(Nodo *lista, int id_buscado, char *palabra_clave);
void liberar_lista(Nodo *lista);
void cargar_tareas(Nodo **pendientes, Nodo **realizadas, int *cant_p, int *cant_r);
void transferir_tareas(Nodo **pendientes, Nodo **realizadas, int *cant_p, int *cant_r);
void consultar_tareas(Nodo *pendientes, Nodo *realizadas);
void listar_tareas(Nodo *pendientes, Nodo *realizadas);
void mostrar_tarea(Nodo *tarea);

// Función principal
int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    srand(time(NULL));

    Nodo *pendientes = NULL;
    Nodo *realizadas = NULL;
    int cant_tareas_pendientes = 0;
    int cant_tareas_realizadas = 0;

    int opcion;
    do
    {
        printf("\nMenú de opciones:\n");
        printf("1. Agregar Tarea\n");
        printf("2. Listar tareas pendientes y realizadas\n");
        printf("3. Consultar tareas por ID o palabra clave\n");
        printf("4. Transferir tareas pendientes a realizadas\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            cargar_tareas(&pendientes, &realizadas, &cant_tareas_pendientes, &cant_tareas_realizadas);
            break;
        case 2:
            listar_tareas(pendientes, realizadas);
            break;
        case 3:
            consultar_tareas(pendientes, realizadas);
            break;
        case 4:
            transferir_tareas(&pendientes, &realizadas, &cant_tareas_pendientes, &cant_tareas_realizadas);
            break;
        case 5:
            printf("Saliendo...\n");
            // Liberar memoria antes de salir
            liberar_lista(pendientes);
            liberar_lista(realizadas);
            break;
        default:
            printf("Opción no válida. Intente nuevamente.\n");
        }
    } while (opcion != 5);

    return 0;
}

// Función para buscar tarea por ID o palabra clave
Nodo *buscar_tarea(Nodo *lista, int id_buscado, char *palabra_clave)
{
    Nodo *actual = lista;

    while (actual != NULL)
    {
        // Si se está buscando por ID
        if (id_buscado != -1 && actual->T.TareaID == id_buscado)
        {
            return actual;
        }
        // Si se está buscando por palabra clave
        if (palabra_clave != NULL && strstr(actual->T.Descripcion, palabra_clave) != NULL)
        {
            return actual;
        }
        actual = actual->Siguiente;
    }

    return NULL; // No se encontró la tarea
}

// Función para liberar la memoria de la lista de tareas
void liberar_lista(Nodo *lista)
{
    Nodo *actual = lista;
    Nodo *siguiente;

    while (actual != NULL)
    {
        siguiente = actual->Siguiente;
        free(actual->T.Descripcion); // Liberar la memoria de la descripción
        free(actual);                // Liberar la memoria del nodo
        actual = siguiente;
    }
}

// Función para agregar una tarea a la lista
void cargar_tareas(Nodo **pendientes, Nodo **realizadas, int *cant_p, int *cant_r)
{
    char descripcion[MAX];
    int duracion;
    Nodo *nuevaTarea;
    int id;

    do
    {
        printf("\nIngrese la descripción de la tarea: ");
        getchar();         // Limpiar el buffer
        gets(descripcion); // Usamos gets para la entrada de la descripción

        printf("Ingrese la duración de la tarea (entre 10 y 100 minutos): ");
        scanf("%d", &duracion);

        id = 1000 + *cant_p + *cant_r; // Generación automática de ID (comienza desde 1000)

        // Crear nuevo nodo
        nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
        nuevaTarea->T.TareaID = id;
        nuevaTarea->T.Descripcion = (char *)malloc(strlen(descripcion) + 1);
        if (nuevaTarea->T.Descripcion != NULL)
        {
            strcpy(nuevaTarea->T.Descripcion, descripcion);
        }
        nuevaTarea->T.Duracion = duracion;
        nuevaTarea->Siguiente = NULL;

        // Insertar la nueva tarea al final de la lista de pendientes
        if (*pendientes == NULL)
        {
            *pendientes = nuevaTarea;
        }
        else
        {
            Nodo *temp = *pendientes;
            while (temp->Siguiente != NULL)
            {
                temp = temp->Siguiente;
            }
            temp->Siguiente = nuevaTarea;
        }

        (*cant_p)++;

        printf("¿Desea ingresar otra tarea? (s/n): ");
        char opcion;
        scanf(" %c", &opcion);
        if (opcion != 's' && opcion != 'S')
        {
            break;
        }
    } while (1);
}

// Función para mostrar la tarea
void mostrar_tarea(Nodo *tarea)
{
    printf("ID: %d\n", tarea->T.TareaID);
    printf("Descripción: %s\n", tarea->T.Descripcion);
    printf("Duración: %d minutos\n", tarea->T.Duracion);
}

// Función para transferir tareas de pendientes a realizadas
void transferir_tareas(Nodo **pendientes, Nodo **realizadas, int *cant_p, int *cant_r)
{
    int id_buscado;
    char opcion;
    Nodo *actual = *pendientes;
    Nodo *anterior = NULL;

    if (actual == NULL)
    {
        printf("\nNo hay tareas pendientes para transferir.\n");
        return;
    }

    do
    {
        printf("\nIngrese el ID de la tarea que desea marcar como realizada: ");
        scanf("%d", &id_buscado);

        actual = *pendientes;
        anterior = NULL;

        while (actual != NULL && actual->T.TareaID != id_buscado)
        {
            anterior = actual;
            actual = actual->Siguiente;
        }

        if (actual == NULL)
        {
            printf("Tarea con ID %d no encontrada en pendientes.\n", id_buscado);
        }
        else
        {
            // Desvincular de pendientes
            if (anterior == NULL)
            {
                *pendientes = actual->Siguiente;
            }
            else
            {
                anterior->Siguiente = actual->Siguiente;
            }

            // Insertar al inicio de realizadas
            actual->Siguiente = *realizadas;
            *realizadas = actual;

            (*cant_p)--;
            (*cant_r)++;

            printf("Tarea con ID %d transferida a realizadas.\n", id_buscado);
        }

        printf("¿Desea transferir otra tarea? (s/n): ");
        scanf(" %c", &opcion);
    } while (opcion == 's' || opcion == 'S');
}

// Función para consultar tareas por ID o palabra clave
void consultar_tareas(Nodo *pendientes, Nodo *realizadas)
{
    int id_buscado;
    char palabra_clave[MAX];
    int opcion;
    Nodo *tarea_encontrada = NULL;

    printf("\nConsultar tareas:\n");
    printf("1. Buscar por ID\n");
    printf("2. Buscar por palabra clave\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);
    getchar();

    if (opcion == 1)
    {
        printf("Ingrese el ID de la tarea a buscar: ");
        scanf("%d", &id_buscado);

        // Buscar en tareas pendientes
        tarea_encontrada = buscar_tarea(pendientes, id_buscado, NULL);
        if (tarea_encontrada != NULL)
        {
            printf("Tarea encontrada en pendientes:\n");
            mostrar_tarea(tarea_encontrada);
        }
        else
        {
            // Buscar en tareas realizadas
            tarea_encontrada = buscar_tarea(realizadas, id_buscado, NULL);
            if (tarea_encontrada != NULL)
            {
                printf("Tarea encontrada en realizadas:\n");
                mostrar_tarea(tarea_encontrada);
            }
            else
            {
                printf("Tarea con ID %d no encontrada.\n", id_buscado);
            }
        }
    }
    else if (opcion == 2)
    {
        printf("Ingrese la palabra clave a buscar: ");
        gets(palabra_clave);
        // Buscar en tareas pendientes
        tarea_encontrada = buscar_tarea(pendientes, -1, palabra_clave);
        if (tarea_encontrada != NULL)
        {
            mostrar_tarea(tarea_encontrada);
        }
        else
        {
            // Buscar en tareas realizadas
            tarea_encontrada = buscar_tarea(realizadas, -1, palabra_clave);
            if (tarea_encontrada != NULL)
            {
                mostrar_tarea(tarea_encontrada);
            }
            else
            {
                printf("No se encontró ninguna tarea con la palabra clave '%s'.\n", palabra_clave);
            }
        }
    }
    else
    {
        printf("Opción no válida.\n");
    }
}

// Función para listar las tareas pendientes y realizadas
void listar_tareas(Nodo *pendientes, Nodo *realizadas)
{
    Nodo *actual;

    // Listar tareas pendientes
    printf("\nTareas Pendientes:\n");
    if (pendientes == NULL)
    {
        printf("No hay tareas pendientes.\n");
    }
    else
    {
        actual = pendientes;
        while (actual != NULL)
        {
            mostrar_tarea(actual);
            actual = actual->Siguiente;
        }
    }
    // Listar tareas realizadas
    printf("\nTareas Realizadas:\n");
    if (realizadas == NULL)
    {
        printf("No hay tareas realizadas.\n");
    }
    else
    {
        actual = realizadas;
        while (actual != NULL)
        {
            mostrar_tarea(actual);
            actual = actual->Siguiente;
        }
    }
}
