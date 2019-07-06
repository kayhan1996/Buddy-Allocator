#include <stdint.h>

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#define MAX_ORDER				11

struct buddy_bucket_t {
	struct page_frame_t *bucket;
	int number;
};

struct buddy_bucket_t buckets[MAX_ORDER];

void init_buddy();
struct page_frame_t * allocate_page_frames(uint64_t order);
void free_page_frames(struct page_frame_t *frames, uint64_t order);
    
#endif