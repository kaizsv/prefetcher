CFLAGS = --std gnu99 -O0 -Wall -g
SSE = -msse2
AVX = -mavx2
PERF = -DPERF
EXEC = naive_transpose sse_transpose sse_prefetch_transpose \
		avx_transpose avx_prefetch_transpose calculate

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
	$(CC) $(CFLAGS) calculate.c -o calculate

diff-distance-perf: all
	for i in `seq 0 1 17` ; do \
		perf stat --repeat 10 -e cache-misses,L1-dcache-load-misses,L1-icache-load-misses ./sse_prefetch_transpose 4096 $$i; \
		perf stat --repeat 10 -e cache-misses,L1-dcache-load-misses,L1-icache-load-misses ./avx_prefetch_transpose 4096 $$i; \
	done;
	./calculate 0
	gnuplot -e "file='diff_distance_out.txt'" ./scripts/runtime.gp

diff-block-size: all
	for i in 1024 2048 4096 8192 ; do \
		perf stat --repeat 10 -e cache-misses,L1-dcache-load-misses,L1-icache-load-misses ./naive_transpose $$i 8; \
		perf stat --repeat 10 -e cache-misses,L1-dcache-load-misses,L1-icache-load-misses ./sse_transpose $$i 8; \
		perf stat --repeat 10 -e cache-misses,L1-dcache-load-misses,L1-icache-load-misses ./sse_prefetch_transpose $$i 8; \
		perf stat --repeat 10 -e cache-misses,L1-dcache-load-misses,L1-icache-load-misses ./avx_transpose $$i 8; \
		perf stat --repeat 10 -e cache-misses,L1-dcache-load-misses,L1-icache-load-misses ./avx_prefetch_transpose $$i 8; \
	done;
	./calculate 1
	gnuplot -e "file='output.txt'" ./scripts/runtime.gp


$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(RM) $(EXEC) *.txt
