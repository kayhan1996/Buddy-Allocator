#include "mm.h"
#include "allocator.h"

int find_index(struct page_frame_t *frame){
	return (frame-AllPages);
}

void init_memory(){
	int i = 0;
	uint64_t address = (uint64_t)memory;

	for(; i < KERNEL_RESERVED; i++){
		AllPages[i].next = NULL;
		AllPages[i].address = address;
		AllPages[i].allocated = 0;
		AllPages[i].kernel = 1;
		address += PAGE_SIZE;
	}

	for(; i < NUM_PAGES; i++){
		AllPages[i].next = NULL;
		AllPages[i].address = address;
		AllPages[i].allocated = 0;
		AllPages[i].kernel = 0;
		address += PAGE_SIZE;
	}

	init_buddy();
}

