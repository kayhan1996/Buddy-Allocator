#include "mm.h"
#include "allocator.h"

/* Finds the index of the frame in the AllPages array */
int find_index(Frame *frame){
	return (frame-AllPages);
}

/* initializes the memory and buddy system */
void init_memory(){
	int i = 0;
	uint64_t address = 0;

	for(; i < KERNEL_RESERVED; i++){
		AllPages[i] = (Frame){
			.node = {NULL, NULL},
			.address = address,
			.allocated = 0,
			.kernel = 1,
			.order = 15
		};
		address += PAGE_SIZE;
	}

	for(; i < NUM_PAGES; i++){
		AllPages[i] = (Frame){
			.node = {NULL, NULL},
			.address = address,
			.allocated = 0,
			.kernel = 0,
			.order = 15
		};
		address += PAGE_SIZE;
	}

	init_buddy();
}

