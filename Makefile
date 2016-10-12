CFLAGS = --std gnu99 -O0 -Wall -g
SSE = -msse2
AVX = -mavx2
EXEC = naive_transpose sse_transpose sse_prefetch_transpose \
		avx_transpose

GIT_HOOKS := .git/hooks/pre-commit

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) $(SSE) -DSSE_PREFETCH_TRANSPOSE main.c -o sse_prefetch_transpose
	$(CC) $(CFLAGS) $(SSE) -DSSE_TRANSPOSE main.c -o sse_transpose
	$(CC) $(CFLAGS) $(SSE) -DNAIVE_TRANSPOSE main.c -o naive_transpose
	$(CC) $(CFLAGS) $(AVX) -DAVX_TRANSPOSE main.c -o avx_transpose

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(RM) $(EXEC) perf*
