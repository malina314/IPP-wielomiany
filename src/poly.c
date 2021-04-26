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
 * Funkcja służy wyłącznie do sprawdzania asercji w kilku funkcjach, których
 * działanie opiera się na założeniu, że wielomiany są posortowane.
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

    res.arr[k++] = monosArr[0];

    for (size_t i = 1; i < count; ++i) {
        if (monosArr[i].exp == res.arr[k - 1].exp) {
            Poly sum = PolyAdd(&res.arr[k - 1].p, &monosArr[i].p);
            MonoDestroy(&res.arr[k - 1]);
            MonoDestroy(&monosArr[i]);
            res.arr[k - 1].p = sum;
        }
        else {
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
 * Mnoży wielomian przez współczynnik.
 * Mnnożony wielomian jest modyfikowany.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] c : współczynnik @f$c@f$
 */
static void PolyMulByCoeff(Poly *p, poly_coeff_t c);

/**
 * Mnoży jednomian przez współczynnik.
 * Mnnożony jednomian jest modyfikowany. Jest to funkcja pomocnicza dla
 * PolyMulByCoeff() i nie jest wywoływana w żadnym innym miejscu.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] c : współczynnik @f$c@f$
 */
static inline void MonoMulByCoeff(Mono *m, poly_coeff_t c) {
    assert(c != 0);
    PolyMulByCoeff(&m->p, c);
}

static void PolyMulByCoeff(Poly *p, poly_coeff_t c) {
    if (c == 0) {
        PolyDestroy(p);
        *p = PolyZero();
    }
    else if (PolyIsCoeff(p)) {
        p->coeff *= c;
    }
    else {
        for (size_t i = 0; i < p->size; ++i) {
            MonoMulByCoeff(&p->arr[i], c);
        }
    }
}

/**
 * Mnoży dwa jednomiany.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$m * n@f$
 */
static inline Mono MonoMul(const Mono *m, const Mono *n) {
    return (Mono) {.p = PolyMul(&m->p, &n->p), .exp = m->exp + n->exp};
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p)) {
        Poly res = PolyClone(q);
        PolyMulByCoeff(&res, p->coeff);
        return res;
    }
    if (PolyIsCoeff(q)) {
        Poly res = PolyClone(p);
        PolyMulByCoeff(&res, q->coeff);
        return res;
    }

    Mono *monos = malloc(p->size * q->size * sizeof (Mono));
    CHECK_PTR(monos);
    size_t k = 0;

    for (size_t i = 0; i < p->size; ++i) {
        for (size_t j = 0; j < q->size; ++j) {
            monos[k++] = MonoMul(&p->arr[i], &q->arr[j]);
        }
    }

    Poly res = PolyAddMonos(k, monos);
    free(monos);

    return res;
}

Poly PolyNeg(const Poly *p) {
    Poly res = PolyClone(p);
    PolyMulByCoeff(&res, -1);
    return res;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly r = PolyNeg(q);
    Poly res = PolyAdd(p, &r);
    PolyDestroy(&r);
    return res;
}

/**
 * Zwraca maksiumum z wartości @f$a, b@f$
 * @param[in] a : liczba @f$a@f$
 * @param[in] b : liczba @f$b@f$
 * @return @f$max(a, b)@f$
 */
static inline size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsZero(p)) {
        return -1;
    }
    if (PolyIsCoeff(p)) {
        return 0;
    }

    if (var_idx == 0) {
        return p->arr[p->size - 1].exp;
    }

    size_t deg = 0;

    for (size_t i = 0; i < p->size; ++i) {
        deg = max(deg, PolyDegBy(&p->arr[i].p, var_idx - 1));
    }

    return deg;
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p)) {
        return -1;
    }
    if (PolyIsCoeff(p)) {
        return 0;
    }

    size_t deg = 0;

    for (size_t i = 0; i < p->size; ++i) {
        deg = max(deg, PolyDeg(&p->arr[i].p) + p->arr[i].exp);
    }

    return deg;
}

/**
 * Oblicza @p n-tą potęgę liczby @p x
 * @param x : podstawa
 * @param n : wykładnik
 * @return @f$x^n@f$
 */
poly_coeff_t fastPow(poly_coeff_t x, poly_exp_t n) {
    poly_coeff_t res = 1;
    while (n) {
        if (n % 2 == 1) {
            res *= x;
        }
        n /= 2;
        x *= x;
    }
    return res;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (x == 0) {
        return PolyZero();
    }
    if (PolyIsCoeff(p)) {
        return PolyFromCoeff(p->coeff * x);
    }

    Poly res = PolyZero();

    for (size_t i = 0; i < p->size; ++i) {
        Poly tmp = PolyClone(&p->arr[i].p);
        PolyMulByCoeff(&tmp, fastPow(x, p->arr[i].exp));
        Poly sum = PolyAdd(&res, &tmp);
        PolyDestroy(&tmp);
        PolyDestroy(&res);
        res = sum;
    }

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

bool PolyIsEq(const Poly *p, const Poly *q) {
    assert(PolyIsSorted(p) && PolyIsSorted(q));

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