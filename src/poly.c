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
#include <string.h>
#include <stdio.h>

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
    Mono *arr = malloc((size) * sizeof (Mono));
    CHECK_PTR(arr);
    return (Poly) {.size = (size), .arr = arr};
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
        for (size_t i = 0; i < p->size; ++i) {
            newPoly.arr[i] = MonoClone(&p->arr[i]);
        }
        return newPoly;
    }
}

/**
 * Sprawdza, czy tablica jednomianów jest posortowana rosnąco po wykładnikach.
 * @param[in] size : rozmiar tablicy
 * @param[in] arr : tablica jednomianów
 * @return Czy tablica jednomianów jest posortowana?
 */
static bool MonoIsArraySorted(size_t size, const Mono *arr) {
    for (size_t i = 1; i < size; ++i) {
        if (arr[i - 1].exp >= arr[i].exp) {
            return false;
        }
    }
    return true;
}

/**
 * Sprawdza, czy jednomiany wielomianu są posortowane rosnąco po wykładniku.
 * Funkcja służy wyłącznie do sprawdzania asercji w kilku funkcjach, których
 * działanie opiera się na założeniu, że wielomiany są posortowane.
 * @param[in] p : wielomian
 * @return Czy wielomian jest posortowany?
 */
#ifndef NDEBUG
static bool PolyIsSorted(const Poly *p) {
    if (!PolyIsCoeff(p)) {
        MonoIsArraySorted(p->size, p->arr);
    }
    return true;
}
#endif

/**
 * Tworzy wielomian składający się tylko z jednego jednomianu.
 * @param[in] m : jednomian
 * @return wielomian
 */
static Poly PolyFormMono(Mono m) {
    Poly p = PolyCreate(1);
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
        if (p->size == 1 && p->arr[0].exp == 0 && PolyIsCoeff(&p->arr[0].p)) {
            // wielomian p ma tylko 1 jednomian stopnia 0, którego wielomian
            // jest współczynnikiem, zatem wielomian p jest współczynnikiem
            Poly tmp = p->arr[0].p;
            free(p->arr);
            *p = tmp;
            return;
        }

        size_t nonZeroCtr = 0;

        for (size_t i = 0; i < p->size; ++i) {
            if (!PolyIsZero(&p->arr[i].p)) {
                nonZeroCtr++;
            }
        }

        if (nonZeroCtr == p->size) {
            return;
        }

        if (nonZeroCtr == 0) {
            free(p->arr);
            *p = PolyZero();
            return;
        }

        size_t k = 0; // indeks tablicy newArr
        Mono *newArr = malloc(nonZeroCtr * sizeof (Mono));
        CHECK_PTR(newArr);

        for (size_t i = 0; i < p->size; ++i) {
            if (!PolyIsZero(&p->arr[i].p)) {
                newArr[k++] = p->arr[i];
            }
        }

        free(p->arr);
        p->arr = newArr;
        p->size = nonZeroCtr;

        if (k == 1 && p->arr[0].exp == 0 && PolyIsCoeff(&p->arr[0].p)) {
            // wielomian p ma tylko 1 jednomian stopnia 0, którego wielomian
            // jest współczynnikiem, zatem wielomian p jest współczynnikiem
            *p = p->arr[0].p;
            free(newArr);
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

    while (i < p->size && j < q->size) {
        if (p->arr[i].exp == q->arr[j].exp) {
            // wykładniki są równe, więc dodajemy
            res.arr[k].exp = p->arr[i].exp;
            res.arr[k++].p = PolyAdd(&p->arr[i++].p, &q->arr[j++].p);
        }
        else if (p->arr[i].exp < q->arr[j].exp) {
            // wykładniki są różne, więc kopiujemy mniejszy jednomian,
            // ponieważ już go nie dodamy, dlatego że tablice są posortowane
            res.arr[k++] = MonoClone(&p->arr[i++]);
        }
        else {
            // j.w.
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
 * @param[in] a : wielomian @f$a@f$
 * @param[in] b : wielomian @f$b@f$
 * @return @f$1@f$ jeśli @f$a@f$ jest większy niż @f$b@f$,
 * @f$0@f$ jeśli są równe,
 * @f$-1@f$ jeśli @f$b@f$ jest większy niż @f$a@f$
 */
static int MonoCompare(const void* a, const void* b) {
    const Mono *arg1 = (const Mono *)a;
    const Mono *arg2 = (const Mono *)b;

    if (arg1->exp > arg2->exp) return 1;
    if (arg1->exp < arg2->exp) return -1;
    return 0;
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    if (count == 0) {
        return PolyZero();
    }
    if (count == 1) {
        if (PolyIsZero(&monos[0].p)) {
            return PolyZero();
        }
        Poly p = PolyFormMono(monos[0]);
        PolyNormalize(&p);
        return p;
    }

    if (!MonoIsArraySorted(count, monos)) {
        qsort(monos, count, sizeof (Mono), MonoCompare);
    }

    Poly res = PolyCreate(count);

    size_t k = 0;

    res.arr[k++] = monos[0];

    for (size_t i = 1; i < count; ++i) {
        if (monos[i].exp == res.arr[k - 1].exp) {
            Poly sum = PolyAdd(&res.arr[k - 1].p, &monos[i].p);
            MonoDestroy(&res.arr[k - 1]);
            MonoDestroy(&monos[i]);
            res.arr[k - 1].p = sum;
        }
        else {
            res.arr[k++] = monos[i];
        }
    }

    if (k < count) {
        PolyShrinkArray(&res, k);
    }

    PolyNormalize(&res);

    free(monos);

    return res;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (count == 0) {
        return PolyZero();
    }

    Mono *monosArr = malloc(count * sizeof (Mono));
    CHECK_PTR(monosArr);
    memcpy(monosArr, monos, count * sizeof (Mono));

    return PolyOwnMonos(count, monosArr);
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    if (count == 0) {
        return PolyZero();
    }

    Mono *monosArr = malloc(count * sizeof (Mono));
    CHECK_PTR(monosArr);

    for (size_t i = 0; i < count; ++i) {
        monosArr[i] = MonoClone(&monos[i]);
    }

    return PolyOwnMonos(count, monosArr);
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
        PolyNormalize(&res);
        return res;
    }
    if (PolyIsCoeff(q)) {
        Poly res = PolyClone(p);
        PolyMulByCoeff(&res, q->coeff);
        PolyNormalize(&res);
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
static inline poly_exp_t Max(poly_exp_t a, poly_exp_t b) {
    return a > b ? a : b;
}

poly_exp_t PolyDegBy(const Poly *p, size_t varIdx) {
    if (PolyIsZero(p)) {
        return -1;
    }
    if (PolyIsCoeff(p)) {
        return 0;
    }

    if (varIdx == 0) {
        return p->arr[p->size - 1].exp;
    }

    poly_exp_t deg = 0;

    for (size_t i = 0; i < p->size; ++i) {
        deg = Max(deg, PolyDegBy(&p->arr[i].p, varIdx - 1));
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

    poly_exp_t deg = 0;

    for (size_t i = 0; i < p->size; ++i) {
        deg = Max(deg, PolyDeg(&p->arr[i].p) + p->arr[i].exp);
    }

    return deg;
}

/**
 * Oblicza @p n-tą potęgę liczby @p x
 * @param[in] x : podstawa
 * @param[in] n : wykładnik
 * @return @f$x^n@f$
 */
poly_coeff_t FastPow(poly_coeff_t x, poly_exp_t n) {
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
    if (PolyIsCoeff(p)) {
        return PolyFromCoeff(p->coeff);
    }

    Poly res = PolyZero();

    for (size_t i = 0; i < p->size; ++i) {
        Poly tmp = PolyClone(&p->arr[i].p);
        PolyMulByCoeff(&tmp, FastPow(x, p->arr[i].exp));
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

/**
 * Wpisuje jednomian zgodnie z przyjętą reprezentacją.
 * @param[in] m : jednomian
 */
static void MonoPrint(const Mono *m) {
    printf("(");
    PolyPrint(&m->p, false);
    printf(",%d)", m->exp);
}

void PolyPrint(const Poly *p, bool newLine) {
    assert(PolyIsSorted(p));

    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    }
    else {
        MonoPrint(&p->arr[0]);
        for (size_t i = 1; i < p->size; ++i) {
            printf("+");
            MonoPrint(&p->arr[i]);
        }
    }

    if (newLine) {
        printf("\n");
    }
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]){
    return *p;
}