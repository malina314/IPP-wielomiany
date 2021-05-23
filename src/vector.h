/** @file
  Plik udostępnia stukturę wektora znaków i wektora jednomianów oraz funkcje
  umożliwiające podstawowe operacje na tych strukturach.

  @authors Mateusz Malinowski
  @date 2021
*/

#ifndef SIMILAR_LINES_VECTOR_H
#define SIMILAR_LINES_VECTOR_H

#include "poly.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * To jest struktura wektora znaków.
 */
typedef struct {
    char *items; ///< tablica przechowująca znaki
    size_t size; ///< liczba elementów na wektorze
    size_t allocated; ///< rozmiar tablicy items
} CVector;

/**
 * To jest struktura wektora jednomianów.
 */
typedef struct {
    Mono *items; ///< tablica przechowująca jednomiany
    size_t size; ///< liczba elementów na wektorze
    size_t allocated; ///< rozmiar tablicy items
} MVector;

/**
 * Alokuje dynamicznie pusty wektor znaków i zwraca wskaźnik na niego.
 * @return wskaźnik na utworzony wektor znaków
 */
CVector *CVectorNew();

/**
 * Zwalnia pamięć używaną przez wektor znaków.
 * @param[in,out] self : wektor znaków
 */
void CVectorFree(CVector *self);

/**
 * Dodaje znak na koniec wektora.
 * @param[in,out] self : wektor znaków
 * @param[in] c : znak
 */
void CVectorPush(CVector *self, char c);

/**
 * Usuwa ostatni element wektora.
 * @param[in,out] self : wektor znaków
 */
void CVectorPop(CVector *self);

/**
 * Zeruje rozmiar wektora, ale nie zwalnia pamięci.
 * @param[in,out] self : wektor znaków
 */
void CVectorClear(CVector *self);

/**
 * Sprawdza, czy wektor jest pusty.
 * @param[in,out] self : wektor znaków
 * @return Czy wektor jest pusty?
 */
bool CVectorEmpty(CVector *self);

/**
 * Tworzy pusty wektor jednomianów i go zwraca.
 * @return wektor jednomianów
 */
MVector MVectorNew();

/**
 * Zwalnia pamięć używaną przez wektor jednomianów.
 * @param[in,out] self : wektor jednomianów
 */
void MVectorFree(MVector *self);

/**
 * Zwalnia pamięć używaną przez wektor jednomianów oraz niszczy te jednomiany.
 * @param[in,out] self : wektor jednomianów
 */
void MVectorDeepFree(MVector *self);

/**
 * Dodaje jednomian na koniec wektora.
 * @param[in,out] self : wektor jednomianów
 * @param[in] m : jednomian
 */
void MVectorPush(MVector *self, Mono m);

#endif //SIMILAR_LINES_VECTOR_H
