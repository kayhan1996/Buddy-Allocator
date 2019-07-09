#include <stdio.h>
#include <allocator.h>
#include <mm.h>
#include <stdlib.h>

int main(){
	AllPages = aligned_alloc(4096, sizeof(Frame) * NUM_PAGES);
	init_memory();

	Frame *frames[6];

	frames[0] = allocate_page_frames(3);
	frames[1] = allocate_page_frames(6);
	frames[2] = allocate_page_frames(4);
	frames[3] = allocate_page_frames(5);
	frames[4] = allocate_page_frames(9);
	frames[5] = allocate_page_frames(1);

	for(int i = 0; i < 6; i++){
		free_page_frames(frames[i]);
		frames[i] = NULL;
	}

	printf("Done\n");
	return 0;
}