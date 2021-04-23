/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych.
  Implementacja zakłada, że jednomiany, z których składa się wielomian są
  posortowane rosnąco po wykładniku.

  @authors Mateusz Malinowski
  @date 2021
*/


#include "poly.h"
#include <stdbool.h>
#include <stdlib.h>


//#define WYPISYWANIE

#ifdef WYPISYWANIE
#include "print.h"
#endif

/**
 * Sprawdza, czy udało się zaalokować pamięć. Jeśli nie, kończy działanie
 * programu z kodem 1.
 * @param[in] p : wskaźnik zwrócony przez funkcję alokującą pamięć
 */
#define CHECK_PTR(p)        \
    do {                    \
        if (p == NULL) {    \
            exit(1);        \
        }                   \
    } while (0)

/**
 * Tworzy wielomian składający się z @p size jednomianów.
 * @param[in] size : rozmiar tablicy jednomianów
 * @return wielomian
 */
static inline Poly PolyCreate(size_t size) {
    return (Poly) {.size = (size), .arr = malloc((size) * sizeof (Mono))};
}

void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; ++i) {
            MonoDestroy(&p->arr[i]);
        }
        free(p->arr);
    }
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) {
        return *p;
    }
    else {
        Poly newPoly = PolyCreate(p->size);
        CHECK_PTR(newPoly.arr);
        for (size_t i = 0; i < p->size; ++i) {
            newPoly.arr[i] = MonoClone(&p->arr[i]);
        }
        return newPoly;
    }
}

/**
 * Sprawdza, czy jednomiany wielomianu są posortowane rosnąco po wykładniku.
 * @param[in] p : wielomian
 * @return Czy wielomian jest posortowany?
 */
static bool PolyIsSorted(const Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 1; i < p->size; ++i) {
            if (p->arr[i - 1].exp >= p->arr[i].exp) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Tworzy wielomian składający się tylko z jednego jednomianu.
 * Przejmuje jednomian na własność.
 * @param[in] m : jednomian
 * @return wielomian
 */
static Poly PolyFormMono(Mono m) {
    Poly p = PolyCreate(1);
    CHECK_PTR(p.arr);
    p.arr[0] = m;
    return p;
}

/**
 * Zmniejsza rozmiar tablicy jednomianów wielomianu.
 * @param[in] p : wielomian
 * @param[in] size : nowy rozmiar
 */
static void PolyShrinkArray(Poly *p, size_t size) {
    assert(!PolyIsCoeff(p) && size <= p->size);
    p->arr = realloc(p->arr, size * sizeof (Mono));
    CHECK_PTR(p->arr);
    p->size = size;
}

/**
 * Usuwa z wielomianu jednomiany o zerowym współczynniku.
 * @param[in] p : wielomian
 */
static void PolyNormalize(Poly *p) {
    if (!PolyIsCoeff(p)) {
        size_t k = 0;
        Mono *newArr = malloc(p->size * sizeof (Mono));
        CHECK_PTR(newArr);

        for (size_t i = 0; i < p->size; ++i) {
            if (!PolyIsCoeff(&p->arr[i].p)) {
                PolyNormalize(&p->arr[i].p);
            }

            if (!PolyIsZero(&p->arr[i].p)) {
                newArr[k++] = p->arr[i];
            }
            else {
                MonoDestroy(&p->arr[i]);
            }
        }

        free(p->arr);
        p->arr = newArr;

        if (k == 0) {
            *p = PolyZero();
            free(newArr);
        }
        else if (k == 1 && p->arr[0].exp == 0 && PolyIsCoeff(&p->arr[0].p)) {
            *p = p->arr[0].p;
            free(newArr);
        }
        else {
            PolyShrinkArray(p, k);
        }
    }
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    assert(PolyIsSorted(p) && PolyIsSorted(q));

    Poly tmp = PolyZero();

    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff + q->coeff);
    }
    else if (PolyIsCoeff(p)) {
        tmp = PolyFormMono(MonoFromPoly(p, 0));
        p = &tmp;
    }
    else if (PolyIsCoeff(q)) {
        tmp = PolyFormMono(MonoFromPoly(q, 0));
        q = &tmp;
    }

    size_t i = 0, j = 0, k = 0;
    Poly res = PolyCreate(p->size + q->size);
    CHECK_PTR(res.arr);

    while (i < p->size && j < q->size) {
        if (p->arr[i].exp == q->arr[j].exp) {
            res.arr[k].exp = p->arr[i].exp;
            res.arr[k++].p = PolyAdd(&p->arr[i++].p, &q->arr[j++].p);
        }
        else if (p->arr[i].exp < q->arr[j].exp) {
            res.arr[k++] = MonoClone(&p->arr[i++]);
        }
        else {
            res.arr[k++] = MonoClone(&q->arr[j++]);
        }
    }

    while (i < p->size) {
        res.arr[k++] = MonoClone(&p->arr[i++]);
    }

    while (j < q->size) {
        res.arr[k++] = MonoClone(&q->arr[j++]);
    }

    if (k < p->size + q->size) {
        PolyShrinkArray(&res, k);
    }

#ifdef WYPISYWANIE
    PolyPrint(&res, 0);
#endif

    PolyNormalize(&res);

    PolyDestroy(&tmp);

    return res;
}

/**
 * Porównuje dwa jednomiany po wykładniku.
 * @param a : wielomian @f$a@f$
 * @param b : wielomian @f$b@f$
 * @return @f$1@f$ jeśli @f$a@f$ jest większy niż @f$b@f$,
 * @f$0@f$ jeśli są równe,
 * @f$-1@f$ jeśli @f$b@f$ jest większy niż @f$a@f$
 */
static int cmpMonos(const void* a, const void* b)
{
    const Mono *arg1 = (const Mono *)a;
    const Mono *arg2 = (const Mono *)b;

    if (arg1->exp > arg2->exp) return 1;
    if (arg1->exp < arg2->exp) return -1;
    return 0;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (count == 0) {
        return PolyZero();
    }

    Mono *monosArr = (Mono *)monos;

    qsort(monosArr, count, sizeof (Mono), cmpMonos);

    Poly res = PolyCreate(count);
    CHECK_PTR(res.arr);

    size_t k = 0;

//    res.arr[k++] = MonoClone(&monosArr[0]);
    res.arr[k++] = monosArr[0];

    for (size_t i = 1; i < count; ++i) {
        if (monosArr[i].exp == res.arr[k - 1].exp) {
            Poly sum = PolyAdd(&res.arr[k - 1].p, &monosArr[i].p);
            MonoDestroy(&res.arr[k - 1]);
            MonoDestroy(&monosArr[i]);
            res.arr[k - 1].p = sum;
        }
        else {
//            res.arr[k++] = MonoClone(&monosArr[i++]);
            res.arr[k++] = monosArr[i];
        }
    }

    if (k < count) {
        PolyShrinkArray(&res, k);
    }

    PolyNormalize(&res);

    return res;
}

/**
 * Sprawdza równość dwóch jednomianów.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$m = n@f$
 */
static inline bool MonoIsEq(const Mono *m, const Mono *n) {
    return m->exp == n->exp && PolyIsEq(&m->p, &n->p);
}

//TODO:
// sprawdzić czy na pewno wielomiany będą znormalizowane
bool PolyIsEq(const Poly *p, const Poly *q) {
    assert(PolyIsSorted(p) && PolyIsSorted(q));

//    PolyNormalize((Poly *)p);
//    PolyNormalize((Poly *)q);

    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return p->coeff == q->coeff;
    }
    else if (!PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        if (p->size != q->size) {
            return false;
        }

        for (size_t i = 0; i < p->size; ++i) {
            if (!MonoIsEq(&p->arr[i], &q->arr[i])) {
                return false;
            }
        }

        return true;
    }
    return false;
}