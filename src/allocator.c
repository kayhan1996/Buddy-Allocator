#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "mm.h"
#include "allocator.h"

#define NOT_PAGE_HEAD								(15)

static inline void add_frame(Frame *frame, int order){
	buckets[order].add(&buckets[order], &frame->node);
}

static inline Frame * get_frame(int order){
	Frame *frame = payload(Frame, buckets[order].remove(&buckets[order]));
	return frame;
}

static inline void remove_frames(Frame *frame){
	buckets[frame->order].remove_node(&buckets[frame->order], &frame->node);
}

static inline Frame * merge_frames(Frame *frame, Frame *buddy){
		remove_frames(buddy);
		if(buddy > frame){
			buddy->order = NOT_PAGE_HEAD;
			frame->order += 1;
			return frame;
		}else{
			buddy->order += 1;
			frame->order = NOT_PAGE_HEAD;
			return buddy;
		}
}

static inline Frame * find_buddy(Frame *frame){
	long b_a = (long)frame->address ^ (1 << (frame->order+12));
	return &AllPages[b_a/PAGE_SIZE];
}

static inline Frame * split_frame(Frame *frame){
	//frame->order--;
	//Frame *buddy = find_buddy(frame);
	Frame *buddy = frame + (1 << (--frame->order));
	add_frame(buddy, buddy->order = frame->order);	
	return frame;
}


Frame * allocate_page_frames(uint64_t order){
	int next_order = order;

	while(buckets[next_order].count == 0)
		next_order++;

	if(next_order > MAX_ORDER) return NULL;

	Frame *frame = get_frame(next_order);

	while(next_order-- != order)
		frame = split_frame(frame);
	
	frame->allocated = 1;

	printf("Allocate frame with address %lx\n", frame->address);

	return frame;
}

void free_page_frames(Frame *frame){
	printf("Freeing %lx\n", frame->address);
	frame->allocated = 0;
	Frame *buddy = find_buddy(frame);
	while(frame->order < MAX_ORDER-1 && frame->order == buddy->order && frame->allocated == 0 && buddy->allocated == 0){
		printf("    <Merging %lx & %lx>\n", frame->address, buddy->address);
		frame = merge_frames(frame, buddy);
		printf("        <Merged to %lx>\n", frame->address);
		buddy = find_buddy(frame);
	}
	add_frame(frame, frame->order);
	
}


void init_buddy(){
	//Zero all buckets
	for(int i = 0; i < MAX_ORDER; i++)
		init_linked_list(&buckets[i]);

	for(int i = 0; i < NUM_PAGES; i+=1024){
		AllPages[i].order = MAX_ORDER - 1;
		buckets[MAX_ORDER-1].add_end(&buckets[MAX_ORDER-1], &AllPages[i].node);
	}
}