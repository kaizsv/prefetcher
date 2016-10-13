CFLAGS = --std gnu99 -O0 -Wall -g
SSE = -msse2
AVX = -mavx2
PERF = -DPERF
EXEC = naive_transpose sse_transpose sse_prefetch_transpose \
		avx_transpose avx_prefetch_transpose

GIT_HOOKS := .git/hooks/pre-commit

ifeq ($(strip $(PERF)),0)
PERF -= -DPERF
endif

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) $(SSE) $(PERF) -DSSE_PREFETCH_TRANSPOSE main.c -o sse_prefetch_transpose
	$(CC) $(CFLAGS) $(SSE) $(PERF) -DSSE_TRANSPOSE main.c -o sse_transpose
	$(CC) $(CFLAGS) $(SSE) $(PERF) -DNAIVE_TRANSPOSE main.c -o naive_transpose
	$(CC) $(CFLAGS) $(AVX) $(PERF) -DAVX_TRANSPOSE main.c -o avx_transpose
	$(CC) $(CFLAGS) $(AVX) $(PERF) -DAVX_PREFETCH_TRANSPOSE main.c -o avx_prefetch_transpose

cache-miss: $(EXEC)
	perf stat --repeat 10 -e cache-misses ./naive_transpose
	perf stat --repeat 10 -e cache-misses ./sse_transpose
	perf stat --repeat 10 -e cache-misses ./sse_prefetch_transpose
	perf stat --repeat 10 -e cache-misses ./avx_transpose
	perf stat --repeat 10 -e cache-misses ./avx_prefetch_transpose

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(RM) $(EXEC) perf* *.txt
