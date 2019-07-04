#include "malloc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MEMORY_SIZE             256
#define MEMORY_SIZE_BYTES       (MEMORY_SIZE * 1024 * 1024)

#define PAGE_SIZE               4096
#define NUM_PAGES               (MEMORY_SIZE_BYTES/PAGE_SIZE)

#define KERNEL_SIZE             16
#define KERNEL_SIZE_BYTES       (KERNEL_SIZE*1024*1024)
#define KERNEL_RESERVED         (KERNEL_SIZE_BYTES/PAGE_SIZE)

typedef struct page_frame_t {
	    struct page_frame_t *next;
	    uint64_t *address;
		uint64_t allocated : 1;
		uint64_t kernel : 1;    
		uint64_t __res : 62;
} PageFrame;

PageFrame *freePages;
uint64_t *memory;

void init_memory(){
	int i = 0;
	uint64_t address = (uint64_t)memory;

#ifdef DEBUG
	uint64_t size = address;
	printf("Kernel reserved %lu pages\n", KERNEL_RESERVED);
	printf("Kernel memory start: %x\n", address);
#endif

	for(; i < KERNEL_RESERVED; i++){
		freePages[i].next = NULL;
		freePages[i].address = address;
		freePages[i].allocated = 1;
		freePages[i].kernel = 1;
		address += PAGE_SIZE;
	}

#ifdef DEBUG
	printf("Kernel memory end: %x\n", address);
	size = -(size-address);
	printf("Size: %x (%luMB)\n\n", size, size/(1024*1024));
	size = address;
	printf("Free memory start: %x\n", address);
#endif

	for(; i < NUM_PAGES; i++){
		freePages[i].next = NULL;
		freePages[i].address = address;
		freePages[i].allocated = 0;
		freePages[i].kernel = 0;
		address += PAGE_SIZE;
	}			    

#ifdef DEBUG
	printf("Free memory end: %x\n", address);
	size = -(size-address);
	printf("Size: %x (%luMB)\n\n", size, size/(1024*1024));
#endif;

}


void *allocate_page_frames(uint32_t num){

}

 int main(){
	printf("Memory size: %luMB\n", MEMORY_SIZE_BYTES/(1024 * 1024));
	printf("Number of pages: %lu\n", NUM_PAGES);
	memory = malloc(MEMORY_SIZE_BYTES);
	freePages = malloc(sizeof(PageFrame) * NUM_PAGES);
	init_memory();
	printf("Hello k World!");
	return 0;
 } 
