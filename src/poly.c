/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych

  @authors Mateusz Malinowski
  @date 2021
*/


#include "poly.h"
#include <stdbool.h>
#include <stdlib.h>

#define POLY(s) {.size = (s), .arr = malloc((s) * sizeof (Mono))}

#define CHECK_PTR(p)        \
    do {                    \
        if (p == NULL) {    \
            exit(1);        \
        }                   \
    } while (0)

void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; ++i) {
            MonoDestroy(&p->arr[i]);
        }
    }
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) {
        return *p;
    }
    else {
        Poly pNew = POLY(p->size);
        CHECK_PTR(pNew.arr);
        for (size_t i = 0; i < p->size; ++i) {
            pNew.arr[i] = MonoClone(&p->arr[i]);
        }
        return pNew;
    }
}

static bool isSorted(const Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 1; i < p->size; ++i) {
            if (p->arr[i - 1].exp >= p->arr[i].exp) {
                return false;
            }
        }
    }
    return true;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    assert(isSorted(p) && isSorted(q));

    // TODO: sprawdzić czy to działa
    if (PolyIsCoeff(p)) {
        Poly tmp = PolyFromCoeff(p->coeff);
        p = &tmp;
    }
    if (PolyIsCoeff(q)) {
        Poly tmp = PolyFromCoeff(q->coeff);
        q = &tmp;
    }

    size_t i = 0, j = 0, k = 0;
    Poly res = POLY(p->size + q->size);
    CHECK_PTR(res.arr);

    while (i < p->size && j < q->size) {
        if (p->arr[i].exp == q->arr[j].exp) {
            res.arr[k].exp = p->arr[i].exp;
            res.arr[k++].p = PolyAdd(&p->arr[i++].p, &q->arr[j++].p);
        }
        else if (p->arr[i].exp < q->arr[j].exp) {
            res.arr[k].exp = p->arr[i].exp;
            res.arr[k++].p = PolyClone(&p->arr[i++].p);
        }
        else {
            res.arr[k].exp = q->arr[i].exp;
            res.arr[k++].p = PolyClone(&q->arr[i++].p);
        }
    }

    while (i < p->size) {
        res.arr[k].exp = p->arr[i].exp;
        res.arr[k++].p = PolyClone(&p->arr[i++].p);
    }

    while (j < q->size) {
        res.arr[k].exp = q->arr[i].exp;
        res.arr[k++].p = PolyClone(&q->arr[i++].p);
    }

    if (k < p->size + q->size) {
        res.arr = realloc(res.arr, k * sizeof (Mono));
        CHECK_PTR(res.arr);
    }

    return res;
}

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

    Poly res = POLY(count);
    CHECK_PTR(res.arr);

    size_t k = 0;

    res.arr[k++] = MonoClone(&monosArr[0]);

    for (size_t i = 1; i < count; ++i) {
        if (monosArr[i].exp == monosArr[i - 1].exp) {
            res.arr[k - 1].p = PolyAdd(&res.arr[k - 1].p, &monosArr[i].p);
        }
        else {
            res.arr[k++] = MonoClone(&monosArr[i++]);
        }
    }

    if (k < count) {
        res.arr = realloc(res.arr, k * sizeof (Mono));
        CHECK_PTR(res.arr);
    }

    return res;
}

//TODO:
bool PolyIsEq(const Poly *p, const Poly *q) {
    return true;
}