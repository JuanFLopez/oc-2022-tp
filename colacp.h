#ifndef COLACP_H_INCLUDED
#define COLACP_H_INCLUDED

typedef void * TClave;
typedef void * TValor;

typedef struct entrada {
	TClave clave;
	TValor valor;
} * TEntrada;

typedef struct nodo {
	TEntrada entrada;
	struct nodo * padre;
	struct nodo * hijo_izquierdo;
	struct nodo * hijo_derecho;
} * TNodo;

typedef struct cola_con_prioridad {
	int cantidad_elementos;
	TNodo raiz;
	int (*comparador)(TEntrada, TEntrada);
} * TColaCP;


/*
 * Crea y retorna una cola con prioridad vacia.
 * El orden en que las entradas deben ser retiradas de la cola estara dado por la funcion de prioridad
 * int f(TEntrada, TEntrada)
 * Se considera que la funcion f devuelve -1 si la clave de la entrada como primer argumento tiene menor
 * prioridad que la clave de la entrada del segundo argumento, 0 si la prioridad es la misma, y 1 si la
 * prioridad es mayor
 */
TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada));

/*
 * Agrega la entrada entr en la cola.
 * Retorna verdadero si procede con exito, falso en caso contrario
 */
int cp_insertar(TColaCP cola, TEntrada entr);

/*
 * Elimina y retorna la entrada con mayor prioridad (esto es, menor clave) de la cola.
 * Reacomoda la estructura heap de forma consistente.
 * Si la cola es vacia, retorna ELE NULO.
 */
TEntrada cp_eliminar(TColaCP cola);

/*
 * Retorna la cantidad de entradas de la cola.
 */
int cp_cantidad(TColaCP cola);

/*
 * Elimina todas las entradas y libera toda la memoria utilizada por la cola cola.
 * Para la eliminacion de las entradas, utiliza la funcion fEliminar
 */
void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada) );

#endif

