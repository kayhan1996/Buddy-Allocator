#include <stdio.h>
#include <allocator.h>
#include <mm.h>
#include <stdlib.h>

Frame *atf(long a){
	int i = a/PAGE_SIZE;
	return &AllPages[i];
}

long x(long a, int o){
	return a ^ (1 << (o+12));
}

Frame *b(Frame *a){
	return atf(x(a->address, a->order));
}



int main(){
	AllPages = aligned_alloc(4096, sizeof(Frame) * NUM_PAGES);
	init_memory();

	Frame *frames[6];

	frames[0] = allocate_page_frames(0);
	frames[1] = allocate_page_frames(5);
	frames[2] = allocate_page_frames(3);
	frames[3] = allocate_page_frames(4);
	frames[4] = allocate_page_frames(1);
	frames[5] = allocate_page_frames(8);

	for(int i = 0; i < 6; i++){
		free_page_frames(frames[i]);
		frames[i] = NULL;
	}

	printf("Done\n");
	return 0;
}