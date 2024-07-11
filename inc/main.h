#ifndef MAIN_H
#define MAIN_H

#include <bcm_host.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

void *map_segment(void *addr, uint32_t size);

#define PAGE_SIZE				0x1000

#endif // !MAIN_H