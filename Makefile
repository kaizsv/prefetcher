CFLAGS = -msse2 --std gnu99 -O0 -Wall
EXEC = naive_transpose sse_transpose sse_prefetch_transpose

GIT_HOOKS := .git/hooks/pre-commit

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE main.c -o sse_prefetch_transpose
	$(CC) $(CFLAGS) -DSSE_TRANSPOSE main.c -o sse_transpose
	$(CC) $(CFLAGS) -DNAIVE_TRANSPOSE main.c -o naive_transpose

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(RM) $(EXEC)
