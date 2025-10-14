#ifndef _INCLUDE_TAR_
#define _INCLUDE_TAR_

int tar_open(char *, int);
size_t tar_write(int, void *, size_t);
size_t tar_read(int, void *, size_t);
void tar_close(int);
void tar_init(void);

#endif
