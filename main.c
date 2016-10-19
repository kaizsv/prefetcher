#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>

#define TEST_W 4096
#define TEST_H 4096

#define USAGE "usage: ./exec [square size] [prefetch distance]"

/* provide the implementations of naive_transpose,
 * sse_transpose, sse_prefetch_transpose
 */

#include "impl.c"

static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main(int argc, char *argv[])
{

    if (argc < 3) {
        printf(USAGE);
        return -1;
    }

    int square_size = atoi(argv[1]);
    int prefetch_distance = atoi(argv[2]);

#ifndef PERF
    /* verify the result of 4x4 matrix */
    {
        int testin[16] = {
            0, 1,  2,  3,  4,  5,  6,  7,
            8, 9, 10, 11, 12, 13, 14, 15
        };
        int testout[16];
        int expected[16] = { 0, 4,  8, 12, 1, 5,  9, 13,
                             2, 6, 10, 14, 3, 7, 11, 15
                           };

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf(" %2d", testin[y * 4 + x]);
            printf("\n");
        }
        printf("\n");
        sse_transpose(testin, testout, 4, 4);
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf(" %2d", testout[y * 4 + x]);
            printf("\n");
        }
        assert(0 == memcmp(testout, expected, 16 * sizeof(int)) &&
               "Verification fails");
    }
#endif

    {
        struct timespec start, end;
        FILE *output;
        int *src  = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
        int *out = (int *) malloc(sizeof(int) * TEST_W * TEST_H);

        srand(time(NULL));
        for (int y = 0; y < square_size; y++)
            for (int x = 0; x < square_size; x++)
                *(src + y * square_size + x) = rand();
#if defined(SSE_PREFETCH_TRANSPOSE)
        output = fopen("sse_prefetch.txt", "a");
        clock_gettime(CLOCK_REALTIME, &start);
        sse_prefetch_transpose(src, out, square_size, square_size, prefetch_distance);
        clock_gettime(CLOCK_REALTIME, &end);
        fprintf(output, "%d %ld\n", prefetch_distance, diff_in_us(start, end));
#endif

#if defined(SSE_TRANSPOSE)
        output = fopen("sse.txt", "a");
        clock_gettime(CLOCK_REALTIME, &start);
        sse_transpose(src, out, square_size, square_size);
        clock_gettime(CLOCK_REALTIME, &end);
        fprintf(output, "%ld\n", diff_in_us(start, end));
#endif

#if defined(NAIVE_TRANSPOSE)
        output = fopen("naive.txt", "a");
        clock_gettime(CLOCK_REALTIME, &start);
        naive_transpose(src, out, square_size, square_size);
        clock_gettime(CLOCK_REALTIME, &end);
        fprintf(output, "%ld\n", diff_in_us(start, end));
#endif

#if defined(AVX_TRANSPOSE)
        output = fopen("avx.txt", "a");
        clock_gettime(CLOCK_REALTIME, &start);
        avx_transpose(src, out, square_size, square_size);
        clock_gettime(CLOCK_REALTIME, &end);
        fprintf(output, "%ld\n", diff_in_us(start, end));
#endif

#if defined(AVX_PREFETCH_TRANSPOSE)
        output = fopen("avx_prefetch.txt", "a");
        clock_gettime(CLOCK_REALTIME, &start);
        avx_prefetch_transpose(src, out, square_size, square_size, prefetch_distance);
        clock_gettime(CLOCK_REALTIME, &end);
        fprintf(output, "%d %ld\n", prefetch_distance, diff_in_us(start, end));
#endif

        fclose(output);
        free(src);
        free(out);
    }

    return 0;
}
