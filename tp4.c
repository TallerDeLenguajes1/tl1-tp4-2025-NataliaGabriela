#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 100
typedef struct
{
    int TareaID;       // Numérico autoincremental comenzando en 1000
    char *Descripcion; //
    int Duracion;      // entre 10 – 100
} Tarea;
typedef struct Nodo
{
    Tarea T;
    struct Nodo *Siguiente;
} Nodo;
Nodo *buscarTarea(Nodo *lista, int id_buscado, char *palabra_clave);
void consultarTareas(Nodo *pendientes, Nodo *realizadas);
void transferirTareasPorId(Nodo **pendientes, Nodo **realizadas);
void mostrarTarea(Nodo *tarea);
void listarTareas(Nodo *pendientes, Nodo *realizadas);
void liberarLista(Nodo *lista);
void agregarTareas(Nodo **lista, int *id);
Tarea cargarTarea(int id);
Nodo *crearNodo(Tarea t);
int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    srand(time(NULL));
    Nodo *tareasPendientes = NULL;
    Nodo *tareasRealizadas = NULL;
    int cant_tareas_pendientes = 0;
    int cant_tareas_realizadas = 0;

    int opcion;
    int id = 1000;
    do
    {
        printf("\nMenú de opciones:\n");
        printf("1. Agregar Tarea\n");
        printf("2. Listar tareas pendientes y realizadas\n");
        printf("3. Transferir tareas pendientes a realizadas\n");
        printf("4. Consultar tareas por ID o palabra clave\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        switch (opcion)
        {
        case 1:
            agregarTareas(&tareasPendientes, &id);
            break;
        case 2:
            listarTareas(tareasPendientes, tareasRealizadas);
            break;
        case 3:
            transferirTareasPorId(&tareasPendientes, &tareasRealizadas);
            break;
        case 4:
            consultarTareas(tareasPendientes, tareasRealizadas);
            break;
        default:
            break;
        }
    } while (opcion != 5);

    liberarLista(tareasPendientes);

    return 0;
}
Nodo *crearNodo(Tarea t)
{
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL)
    {
        printf("Error al asignar memoria para el nodo.\n");
        exit(1);
    }
    nuevo->T = t;
    nuevo->Siguiente = NULL;
    return nuevo;
}
Tarea cargarTarea(int id)
{
    Tarea nuevaT;
    char *buff = (char *)malloc(sizeof(char) * 100); // RECORDAR LIBERAR. puntero auxiliar para descripcion
    nuevaT.TareaID = id;
    printf("\nIngrese la descripcion de la tarea: ");
    fflush(stdin);
    gets(buff);                                                             // lee descripcion en buff
    nuevaT.Descripcion = (char *)malloc((strlen(buff) + 1) * sizeof(char)); // RECORDAR LIBERAR. reservo memoria para la descripcion
    strcpy(nuevaT.Descripcion, buff);                                       // copia el contenido de buff a descripcion
    do
    {
        printf("\nIngrese la duracion de la tarea (entre 10 y 100): ");
        scanf("%d", &nuevaT.Duracion);
    } while (nuevaT.Duracion < 10 || nuevaT.Duracion > 100);
    free(buff);
    return nuevaT;
}
// agrego tareas nodo al comienzo de la lista
void agregarTareas(Nodo **lista, int *id)
{
    int decision;
    do
    {
        Tarea nueva = cargarTarea(*id);
        Nodo *nuevoNodo = crearNodo(nueva);
        nuevoNodo->Siguiente = *lista;
        *lista = nuevoNodo;
        (*id)++;

        printf("\nDesea cargar otra tarea?");
        printf("\n[0]. No");
        printf("\n[1]. Si");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &decision);
    } while (decision != 0);
}
void mostrarTarea(Nodo *tarea)
{
    printf("ID: %d\n", tarea->T.TareaID);
    printf("Descripción: %s\n", tarea->T.Descripcion);
    printf("Duración: %d minutos\n", tarea->T.Duracion);
}
void listarTareas(Nodo *pendientes, Nodo *realizadas)
{
    Nodo *aux;
    printf("\nTareas Pendientes:\n");
    if (pendientes == NULL)
    {
        printf("No hay tareas pendientes.\n");
    }
    else
    {
        aux = pendientes;
        while (aux != NULL)
        {
            mostrarTarea(aux);
            aux = aux->Siguiente;
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
        aux = realizadas;
        while (aux != NULL)
        {
            mostrarTarea(aux);
            aux = aux->Siguiente;
        }
    }
}
void transferirTareasPorId(Nodo **pendientes, Nodo **realizadas)
{
    int id, decision;
    do
    {
        printf("\nIngrese el id de la tarea que desea agregar a la lista de tareas realizadas: ");
        scanf("%d", &id); // Lee el ID de la tarea a transferir.
        Nodo *actual = *pendientes;
        Nodo *anterior = NULL;

        // Busca la tarea en la lista de tareas pendientes.
        while (actual != NULL && actual->T.TareaID != id)
        {
            anterior = actual;
            actual = actual->Siguiente;
        }

        if (actual != NULL) // Si la tarea fue encontrada.
        {
            if (anterior != NULL)
            {
                anterior->Siguiente = actual->Siguiente; // Elimina el nodo de la lista de pendientes.
            }
            else
            {
                *pendientes = actual->Siguiente; // Si es el primer nodo, lo elimina cambiando el puntero de la lista.
            }
            actual->Siguiente = *realizadas; // El nodo se inserta al principio de la lista de realizadas.
            *realizadas = actual;            // El primer nodo de la lista de realizadas pasa a ser el nodo transferido.
        }
        else
        {
            printf("No se encontro una tarea con ese ID en la lista de tareas pendientes");
        }

        // Pregunta al usuario si desea continuar transfiriendo tareas.
        printf("\nDesea continuar transfiriendo tareas?");
        printf("\n[0]. No");
        printf("\n[1]. Si");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &decision);
    } while (decision != 0); // El bucle continúa hasta que el usuario elija no transferir más tareas.
}
Nodo *buscarTarea(Nodo *lista, int id_buscado, char *palabra_clave)
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
void consultarTareas(Nodo *pendientes, Nodo *realizadas)
{
    int idBuscado;
    char palabraClave[MAX];
    int opcion;
    Nodo *tareaEncontrada = NULL;

    printf("\nConsultar tareas:\n");
    printf("1. Buscar por ID\n");
    printf("2. Buscar por palabra clave\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);
    getchar();

    if (opcion == 1)
    {
        printf("Ingrese el ID de la tarea a buscar: ");
        scanf("%d", &idBuscado);

        // Buscar en tareas pendientes
        tareaEncontrada = buscarTarea(pendientes, idBuscado, NULL);
        if (tareaEncontrada != NULL)
        {
            printf("Tarea encontrada en pendientes:\n");
            mostrarTarea(tareaEncontrada);
        }
        else
        {
            // Buscar en tareas realizadas
            tareaEncontrada = buscarTarea(realizadas, idBuscado, NULL);
            if (tareaEncontrada != NULL)
            {
                printf("Tarea encontrada en realizadas:\n");
                mostrarTarea(tareaEncontrada);
            }
            else
            {
                printf("Tarea con ID %d no encontrada.\n", idBuscado);
            }
        }
    }
    else if (opcion == 2)
    {
        printf("Ingrese la palabra clave a buscar: ");
        gets(palabraClave);
        // Buscar en tareas pendientes
        tareaEncontrada = buscarTarea(pendientes, -1, palabraClave);
        if (tareaEncontrada != NULL)
        {
            mostrarTarea(tareaEncontrada);
        }
        else
        {
            // Buscar en tareas realizadas
            tareaEncontrada = buscarTarea(realizadas, -1, palabraClave);
            if (tareaEncontrada != NULL)
            {
                mostrarTarea(tareaEncontrada);
            }
            else
            {
                printf("No se encontró ninguna tarea con la palabra clave '%s'.\n", palabraClave);
            }
        }
    }
    else
    {
        printf("Opción no válida.\n");
    }
}
void liberarLista(Nodo *lista)
{
    Nodo *aux;
    while (lista != NULL)
    {
        aux = lista;
        lista = lista->Siguiente;
        free(aux->T.Descripcion);
        free(aux);
    }
}
