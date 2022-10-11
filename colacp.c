#include "colacp.h"
#include "define.h"
#include <stdlib.h>

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada))
{
	TColaCP cola = malloc(sizeof(struct cola_con_prioridad));

	cola->cantidad_elementos = 0;
	cola->raiz = NULL;
	cola->comparador = f;

	return cola;
}

static void cp_rearmar_abajo(TColaCP cola, TNodo nodo)
{
	TEntrada aux = NULL;
	TNodo mayor = NULL;

	if (nodo->hijo_izquierdo && cola->comparador(nodo->hijo_izquierdo->entrada, nodo->entrada) > 0) {
		mayor = nodo->hijo_izquierdo;
	} else if (nodo->hijo_derecho && cola->comparador(nodo->hijo_derecho->entrada, nodo->entrada) > 0) {
		mayor = nodo->hijo_derecho;
	}
	if (mayor != NULL) {
		aux = nodo->entrada;
		nodo->entrada = mayor->entrada;
		mayor->entrada = aux;
		cp_rearmar_abajo(cola, mayor);
	}
}

static int cp_altura_fila_completa(const TNodo nodo)
{
	if (nodo == NULL) {
		return 0;
	}

	//el arbol se completa de izquierda a derecha
	return 1 + cp_altura_fila_completa(nodo->hijo_derecho);
}

static TNodo cp_ultimo_nodo_libre(TNodo nodo)
{
	int altura_derecho;
	int altura_izquierdo;

	/*
	 * caso base: si al nodo le falta algun hijo, este es el ultimo nodo libre.
	 *            como el arbol se completa de izquierda a derecha, no puede darse que
	 *            tenga hijo derecho pero no izquierdo.
	 *            o bien le faltan ambos hijos, o bien solo le falta el derecho
	 */
	if (nodo->hijo_derecho == NULL) {
		return nodo;
	}

	altura_izquierdo = cp_altura_fila_completa(nodo->hijo_izquierdo);
	altura_derecho = cp_altura_fila_completa(nodo->hijo_derecho);

	/*
	 * asumiendo que cp_ultimo_nodo_libre(hijo) es correcto:
	 *            cp_altura_fila_completa() retorna la altura de la ultima fila completa del arbol
	 *            como se llena de izquierda a derecha, no puede ser que por el hijo derecho tenga mas
	 *            altura que desde el hijo izquierdo.
	 *
	 *            - si el hijo derecho tiene menor altura que el hijo izquierdo, es que por la izquierda ya
	 *            esta completa toda la fila, entonces el ultimo nodo libre esta por la derecha:
	 *            cp_ultimo_nodo_libre(nodo->hijo_derecho)
	 *
	 *            - si tienen la misma altura, o bien el arbol esta completo o bien por izquierda aun se esta
	 *            completando y tiene algun nodo tiene que llenarse con un hijo.
	 *            en ambos casos el ultimo nodo libre esta por izquierda:
	 *            cp_ultimo_nodo_libre(nodo->hijo_izquierdo)
	 */
	/* Arbol completo, misma altura (2) por izquierda (B) y derecha (C), hay que completar por izquierda (D)
	 *      A
	 *     / \
	 *    B   C
	 *   / \ / \
	 *  D  E F  G
	 */
	/* Arbol incompleto, misma altura (1) por izquierda (B) y derecha (C), hay que completar por izquierda (B)
	 *      A
	 *     / \
	 *    B   C
	 *   /
	 *  D
	 */
	/* Arbol incompleto, diferente altura (2 vs 1), hay que completar por derecha (C)
	 *      A
	 *     / \
	 *    B   C
	 *   / \ /
	 *  D  E F
	 */

	if (altura_derecho < altura_izquierdo) {
		return cp_ultimo_nodo_libre(nodo->hijo_derecho);
	} else {
		return cp_ultimo_nodo_libre(nodo->hijo_izquierdo);
	}
}

static void cp_rearmar_arriba(TColaCP cola, TNodo nodo)
{
	TEntrada aux = NULL;

	if (nodo->padre != NULL && cola->comparador(nodo->entrada, nodo->padre->entrada) > 0) {
		aux = nodo->entrada;
		nodo->entrada = nodo->padre->entrada;
		nodo->padre->entrada = aux;
		cp_rearmar_arriba(cola, nodo->padre);
	}
}

int cp_insertar(TColaCP cola, TEntrada entr)
{
	TNodo nodo = NULL;
	TNodo ultimo = NULL;

	if (cola == NULL) {
		exit(CPP_NO_INI);
	}
	if (entr == NULL) {
		return FALSE;
	}

	nodo = malloc(sizeof(struct nodo));
	nodo->entrada = entr;
	nodo->padre = NULL;
	nodo->hijo_derecho = NULL;
	nodo->hijo_izquierdo = NULL;

	if (cola->raiz == NULL) {
		cola->raiz = nodo;
	} else {
		ultimo = cp_ultimo_nodo_libre(cola->raiz);
		nodo->padre = ultimo;
		if (ultimo->hijo_izquierdo == NULL) {
			ultimo->hijo_izquierdo = nodo;
		} else {
			ultimo->hijo_derecho = nodo;
		}
		cp_rearmar_arriba(cola, nodo);
	}
	cola->cantidad_elementos++;

	return TRUE;
}

