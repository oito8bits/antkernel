#include <arch/x86_64/context.h>
#include <kernel/getrusage.h>

int wait_wait4(int, int *, int, struct rusage *, struct context *);
