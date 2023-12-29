#include <stdio.h>
#include <stdlib.h>
#include "../src/galois.h"
#include "../third_party/gf.h"

void test_gf(int m) {
    gf_init(m, prim_poly[m]);
    galois_init(m);

    // test add operation
    for (int i = 0; i < 1 << m; ++i) {
        for (int j = 0; j < 1 << m; ++j) {
            if (galois_add(i, j) != gf_add(i, j)) {
                printf("i = %d, j = %d\n", i, j);
                printf("galois_add(i, j) = %d %d\n", galois_add(i, j), gf_add(i, j));
                exit(1);
            }
        }
    }

    // test mul operation
    for (int i = 0; i < 1 << m; ++i) {
        for (int j = 0; j < 1 << m; ++j) {
            if (galois_mul(i, j) != gf_mul(i, j)) {
                printf("i = %d, j = %d\n", i, j);
                printf("galois_mul(i, j) = %d %d\n", galois_mul(i, j), gf_mul(i, j));
                exit(1);
            }
        }
    }

    // test div operation
    for (int i = 0; i < 1 << m; ++i) {
        for (int j = 0; j < 1 << m; ++j) {
            if (galois_div(i, j) != gf_div(i, j)) {
                printf("i = %d, j = %d\n", i, j);
                exit(1);
            }
        }
    }

    printf("gf(2^%d) \033[32msuccess\033[0m\n", m);

    gf_uninit();
    free_galois();
}

int main() {
    for (int i = 1; i <= 8; ++i) {
        test_gf(i);
    }

    return 0;
}