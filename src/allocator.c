#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "mm.h"
#include "allocator.h"

static inline void add_frame(struct page_frame_t *frame, int order){
	frame->next = buckets[order].bucket;
	buckets[order].bucket = frame;
	buckets[order].number += 1;
}

static inline struct page_frame_t * get_frame(int order){
	struct page_frame_t *frame = buckets[order].bucket;
	buckets[order].bucket = frame->next;
	buckets[order].number -= 1;
	return frame;
}

static inline struct page_frame_t * split_frame(struct page_frame_t *frame, int order){
	struct page_frame_t *buddy = frame + (1 << (order - 1));
	add_frame(buddy, order - 1);
	uint64_t size = (buddy->address - frame->address);
	printf("Frame: %lx, Buddy: %lx, size: %ldKB, frames: %d\n", frame, buddy, size/1024, size/PAGE_SIZE);
	return frame;
}

static inline int check_contiguous(struct page_frame_t *frame[], int number, int order){

	uint64_t spacing = (1 << order) * PAGE_SIZE;

	uint64_t prev_address = frame[0]->address;
	uint64_t next_address;
	for(int i = 1; i < number; i++){
		next_address = frame[i]->address;
		if( (prev_address + spacing) != next_address){
			return -1;
		}
		prev_address = next_address;
	}
	return 1;
}

void init_buddy(){

	for(int i = 0; i < MAX_ORDER; i++){
		buckets[i].bucket = NULL;
		buckets[i].number = 0;
	}

	buckets[MAX_ORDER-1].bucket = &AllPages[0];
	struct page_frame_t * current = buckets[MAX_ORDER-1].bucket;
	buckets[MAX_ORDER-1].number += 1;

	long previous = current->address;
	long next;

	for(int i = 1024; i < NUM_PAGES; i+=1024){
		
		current->next = &AllPages[i];
		current = current->next;
		buckets[MAX_ORDER-1].number += 1;
		//DEBUG
		next = current->address;
		long page_size = next - previous;
		previous = next;
	}
}

int main(){
	memory = malloc(MEMORY_SIZE_BYTES);
	AllPages = malloc(sizeof(PageFrame) * NUM_PAGES);

	struct page_frame_t* test_frames[MAX_ORDER][32];
	init_memory();
	for(int i = 0; i < MAX_ORDER; i++){
		for(int j = 0; j < 32; j++){
			test_frames[i][j] = allocate_page_frames(i);
		}
	}

	uint64_t prev;
	for(int i = 0; i < MAX_ORDER; i++){
		printf("Order %d\n", i);
		for(int j = 0; j < 32; j+=2){
			printf("	test[%d] address: 0x%x\n",j, test_frames[i][j]->address);
			printf("	test[%d] address: 0x%x\n",j+1, test_frames[i][j+1]->address);
			printf("		diff: 0x%lx\n", test_frames[i][j+1]->address - test_frames[i][j]->address);
		}
		printf("\n\n");
	}

	free_page_frames(test_frames[5][0], 5);

	return 0;
}

struct page_frame_t * allocate_page_frames(uint64_t order){
	printf("----------Allocating Page Frames----------\n");
	int next_order = order;

	while(buckets[next_order].number == 0){
		printf("No frames of order %d\n", next_order);
		next_order += 1;
	}

	if(next_order > MAX_ORDER){
		printf("allocation error!\n");
		return NULL;
	}

	printf("Frames of order %d found!\n", next_order);

	struct page_frame_t *frame = get_frame(next_order);

	while(next_order != order){
		frame = split_frame(frame, next_order--);
	}

	printf("Page frames allocated!\n\n\n");

	frame->allocated = 1;

	return frame;
}

void free_page_frames(struct page_frame_t *frames, uint64_t order){
	add_frame(frames, order);
	if(frames->address){
		return;
	}
}
