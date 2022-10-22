#include "colacp.h"
#include "define.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ciudad {
    char *nombre;
    float pos_x;
    float pos_y;
} *TCiudad;

int comparar_mayor(TEntrada a, TEntrada b){
    float valora = *(float *)a->valor;
    float valorb = *(float *)b->valor;
    if(valora > valorb){
        return 1;
    }
    if(valorb > valora){
        return -1;
    }
    return 0;
}

int comparar_menor(TEntrada a, TEntrada b){
    return comparar_mayor(b, a);
}

TCiudad crear_ciudad(const char *nombre, float pos_x, float pos_y){
    TCiudad c;
    c = malloc(sizeof(*c));
    c->nombre = strdup(nombre);
    c->pos_x = pos_x;
    c->pos_y = pos_y;
    return c;
}

void free_ciudad(TCiudad c){
    free(c->nombre);
    free(c);
}

TEntrada crear_entrada(TCiudad c, float distancia){
    TEntrada e;
    e = malloc(sizeof(*e));
    e->clave = c;
    e->valor = malloc(sizeof(float));
    *(float *)e->valor = distancia;
    return e;
}

void free_entrada(TEntrada e){
    free(e->valor);
    free(e);
}

TCiudad *cargar_ciudades(const char *entrada, float *actual_x, float *actual_y, int *cantidad){
    FILE *f;
    char nombre[1000] = {0};
    float pos_x, pos_y;
    TCiudad *ciudades = NULL;

    f = fopen(entrada, "r");
    if(!f){
        printf("No existe el archivo %s\n", entrada);
        return NULL;
    }
    if(fscanf(f, "%f;%f", actual_x, actual_y) != 2){
        printf("Se esperaba la posicion actual como primera linea en el archivo %s\n", entrada);
    }
    else{
        *cantidad = 0;
        while(!feof(f)){
            if(fscanf(f, "%999[^;];%f;%f", nombre, &pos_x, &pos_y) != 3){
                printf("Se esperaba que la linea tenga 3 campos: nombre;posx;posy en el archivo %s\n", entrada);
                break;
            }
            ciudades = realloc(ciudades, (1 + *cantidad) * sizeof(TCiudad));
            ciudades[*cantidad] = crear_ciudad(nombre, pos_x, pos_y);
            (*cantidad)++;
        }
    }

    fclose(f);
    return ciudades;
}

void liberar_ciudades(TCiudad *ciudades, int cantidad){
    int i;
    for(i = 0; i < cantidad; i++){
        free_ciudad(ciudades[i]);
    }
    free(ciudades);
}

void mostrar_ascendente(TCiudad *ciudades, int cantidad, float actual_x, float actual_y){
    TColaCP ccp;
    TEntrada e;
    int i;
    char *nombre;

    ccp = crear_cola_cp(comparar_menor);
    for(i = 0; i < cantidad; i++){
        e = crear_entrada(ciudades[i], fabs(actual_x - ciudades[i]->pos_x) + fabs(actual_y - ciudades[i]->pos_y));
        cp_insertar(ccp, e);
    }
    i = 0;
    for(e = cp_eliminar(ccp); e != ELE_NULO; e = cp_eliminar(ccp)){
        nombre = ((TCiudad)e->clave)->nombre;
        printf("%d. %s.\n", i, nombre);
    }
    cp_destruir(ccp, free_entrada);
}

void menu(TCiudad *ciudades, int cantidad, float actual_x, float actual_y)
{
    int opcion = 0;
    while(opcion != 4){
        printf("\nMenu:\n"
               "\t1. Mostrar ascendente\n"
               "\t2. Mostrar descendente\n"
               "\t3. Reducir horas manejo\n"
               "\t4. Salir\n\nElegir opcion [1-4]: ");
        opcion = 0;
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                mostrar_ascendente(ciudades, cantidad, actual_x, actual_y);
                break;
            case 2:
                //mostrar_descendete(ciudades, cantidad, actual_x, actual_y);
                break;
            case 3:
                //reducir_horas_manejo(ciudades, cantidad, actual_x, actual_y);
                break;
            case 4:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    }
}

int main(int argc, char **argv)
{
    const char *entrada;
    TCiudad *ciudades = NULL;
    int cantidad = 0;
    float actual_x = 0, actual_y = 0;

    if(argc != 2){
        printf("Falta archivo de entrada.\n"
               "Uso:\n"
               "%s <archivo_texto>\n", argv[0]);
        return -1;
    }
    entrada = argv[1];

    ciudades = cargar_ciudades(entrada, &actual_x, &actual_y, &cantidad);
    if(ciudades == NULL){
        printf("Error cargando el archivo de entrada %s\n", entrada);
    }
    else{
        menu(ciudades, cantidad, actual_x, actual_y);
    }
    liberar_ciudades(ciudades, cantidad);

    return 0;
}

