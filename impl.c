#ifndef TRANSPOSE_IMPL
#define TRANSPOSE_IMPL

#include <immintrin.h>

void naive_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            *(dst + x * h + y) = *(src + y * w + x);
}

void sse_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128i I0 = _mm_loadu_si128((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128((__m128i *)(src + (y + 3) * w + x));
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}

void sse_prefetch_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
#define PFDIST  8
            _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T1);

            __m128i I0 = _mm_loadu_si128 ((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128 ((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128 ((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128 ((__m128i *)(src + (y + 3) * w + x));
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}

#if defined(AVX_TRANSPOSE)
void avx_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 8) {
        for (int y = 0; y < h; y += 8) {
            __m256i ymm0 = _mm256_loadu_si256((__m256i *) (src + (y + 0) * w + x));
            __m256i ymm1 = _mm256_loadu_si256((__m256i *) (src + (y + 1) * w + x));
            __m256i ymm2 = _mm256_loadu_si256((__m256i *) (src + (y + 2) * w + x));
            __m256i ymm3 = _mm256_loadu_si256((__m256i *) (src + (y + 3) * w + x));
            __m256i ymm4 = _mm256_loadu_si256((__m256i *) (src + (y + 4) * w + x));
            __m256i ymm5 = _mm256_loadu_si256((__m256i *) (src + (y + 5) * w + x));
            __m256i ymm6 = _mm256_loadu_si256((__m256i *) (src + (y + 6) * w + x));
            __m256i ymm7 = _mm256_loadu_si256((__m256i *) (src + (y + 7) * w + x));

            __m256i t0 = _mm256_unpacklo_epi32(ymm0, ymm1);
            __m256i t1 = _mm256_unpacklo_epi32(ymm2, ymm3);
            __m256i t2 = _mm256_unpackhi_epi32(ymm0, ymm1);
            __m256i t3 = _mm256_unpackhi_epi32(ymm2, ymm3);
            __m256i t4 = _mm256_unpacklo_epi32(ymm4, ymm5);
            __m256i t5 = _mm256_unpacklo_epi32(ymm6, ymm7);
            __m256i t6 = _mm256_unpackhi_epi32(ymm4, ymm5);
            __m256i t7 = _mm256_unpackhi_epi32(ymm6, ymm7);

            ymm0 = _mm256_unpacklo_epi64(t0, t1);
            ymm1 = _mm256_unpackhi_epi64(t0, t1);
            ymm2 = _mm256_unpacklo_epi64(t2, t3);
            ymm3 = _mm256_unpackhi_epi64(t2, t3);
            ymm4 = _mm256_unpacklo_epi64(t4, t5);
            ymm5 = _mm256_unpackhi_epi64(t4, t5);
            ymm6 = _mm256_unpacklo_epi64(t6, t7);
            ymm7 = _mm256_unpacklo_epi64(t6, t7);

            t0 = _mm256_permute2x128_si256(ymm0, ymm4, 0x20);
            t1 = _mm256_permute2x128_si256(ymm1, ymm5, 0x20);
            t2 = _mm256_permute2x128_si256(ymm2, ymm6, 0x20);
            t3 = _mm256_permute2x128_si256(ymm3, ymm7, 0x20);
            t4 = _mm256_permute2x128_si256(ymm0, ymm4, 0x31);
            t5 = _mm256_permute2x128_si256(ymm1, ymm5, 0x31);
            t6 = _mm256_permute2x128_si256(ymm2, ymm6, 0x31);
            t7 = _mm256_permute2x128_si256(ymm3, ymm7, 0x31);

            _mm256_storeu_si256((__m256i *) (dst + (x + 0) * h + y), t0);
            _mm256_storeu_si256((__m256i *) (dst + (x + 1) * h + y), t1);
            _mm256_storeu_si256((__m256i *) (dst + (x + 2) * h + y), t2);
            _mm256_storeu_si256((__m256i *) (dst + (x + 3) * h + y), t3);
            _mm256_storeu_si256((__m256i *) (dst + (x + 4) * h + y), t4);
            _mm256_storeu_si256((__m256i *) (dst + (x + 5) * h + y), t5);
            _mm256_storeu_si256((__m256i *) (dst + (x + 6) * h + y), t6);
            _mm256_storeu_si256((__m256i *) (dst + (x + 7) * h + y), t7);
        }
    }
}
#endif

#if defined(AVX_PREFETCH_TRANSPOSE)
void avx_prefetch_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 8) {
        for (int y = 0; y < h; y += 8) {
#define AVX_PFDIST 8
            _mm_prefetch(src+(y + AVX_PFDIST + 0) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + AVX_PFDIST + 1) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + AVX_PFDIST + 2) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + AVX_PFDIST + 3) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + AVX_PFDIST + 4) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + AVX_PFDIST + 5) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + AVX_PFDIST + 6) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + AVX_PFDIST + 7) *w + x, _MM_HINT_T1);

            __m256i ymm0 = _mm256_loadu_si256((__m256i *) (src + (y + 0) * w + x));
            __m256i ymm1 = _mm256_loadu_si256((__m256i *) (src + (y + 1) * w + x));
            __m256i ymm2 = _mm256_loadu_si256((__m256i *) (src + (y + 2) * w + x));
            __m256i ymm3 = _mm256_loadu_si256((__m256i *) (src + (y + 3) * w + x));
            __m256i ymm4 = _mm256_loadu_si256((__m256i *) (src + (y + 4) * w + x));
            __m256i ymm5 = _mm256_loadu_si256((__m256i *) (src + (y + 5) * w + x));
            __m256i ymm6 = _mm256_loadu_si256((__m256i *) (src + (y + 6) * w + x));
            __m256i ymm7 = _mm256_loadu_si256((__m256i *) (src + (y + 7) * w + x));

            __m256i t0 = _mm256_unpacklo_epi32(ymm0, ymm1);
            __m256i t1 = _mm256_unpacklo_epi32(ymm2, ymm3);
            __m256i t2 = _mm256_unpackhi_epi32(ymm0, ymm1);
            __m256i t3 = _mm256_unpackhi_epi32(ymm2, ymm3);
            __m256i t4 = _mm256_unpacklo_epi32(ymm4, ymm5);
            __m256i t5 = _mm256_unpacklo_epi32(ymm6, ymm7);
            __m256i t6 = _mm256_unpackhi_epi32(ymm4, ymm5);
            __m256i t7 = _mm256_unpackhi_epi32(ymm6, ymm7);

            ymm0 = _mm256_unpacklo_epi64(t0, t1);
            ymm1 = _mm256_unpackhi_epi64(t0, t1);
            ymm2 = _mm256_unpacklo_epi64(t2, t3);
            ymm3 = _mm256_unpackhi_epi64(t2, t3);
            ymm4 = _mm256_unpacklo_epi64(t4, t5);
            ymm5 = _mm256_unpackhi_epi64(t4, t5);
            ymm6 = _mm256_unpacklo_epi64(t6, t7);
            ymm7 = _mm256_unpacklo_epi64(t6, t7);

            t0 = _mm256_permute2x128_si256(ymm0, ymm4, 0x20);
            t1 = _mm256_permute2x128_si256(ymm1, ymm5, 0x20);
            t2 = _mm256_permute2x128_si256(ymm2, ymm6, 0x20);
            t3 = _mm256_permute2x128_si256(ymm3, ymm7, 0x20);
            t4 = _mm256_permute2x128_si256(ymm0, ymm4, 0x31);
            t5 = _mm256_permute2x128_si256(ymm1, ymm5, 0x31);
            t6 = _mm256_permute2x128_si256(ymm2, ymm6, 0x31);
            t7 = _mm256_permute2x128_si256(ymm3, ymm7, 0x31);

            _mm256_storeu_si256((__m256i *) (dst + (x + 0) * h + y), t0);
            _mm256_storeu_si256((__m256i *) (dst + (x + 1) * h + y), t1);
            _mm256_storeu_si256((__m256i *) (dst + (x + 2) * h + y), t2);
            _mm256_storeu_si256((__m256i *) (dst + (x + 3) * h + y), t3);
            _mm256_storeu_si256((__m256i *) (dst + (x + 4) * h + y), t4);
            _mm256_storeu_si256((__m256i *) (dst + (x + 5) * h + y), t5);
            _mm256_storeu_si256((__m256i *) (dst + (x + 6) * h + y), t6);
            _mm256_storeu_si256((__m256i *) (dst + (x + 7) * h + y), t7);
        }
    }
}
#endif

#endif /* TRANSPOSE_IMPL */
