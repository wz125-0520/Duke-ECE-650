#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

typedef struct meta{
  size_t block_size;
  struct meta* next;
}metadata;
metadata* freed_head;

unsigned long space = 0;



//metadata** search_block(metadata** current, size_t realsize);
void malloc_freed(metadata** current, size_t realsize);
void* ff_malloc(size_t size);
void* ff_free(void *ptr);
//void add_first(metadata** current, metadata* ptr_block);
//void merge_middle(metadata** current, metadata* ptr_block);
//void merge_front(metadata** current, metadata* ptr_block);
void merge( metadata* ptr_block);
void add_new(metadata* ptr_block);
//void scan_freed(metadata** current, metadata* ptr_block);
void* bf_malloc(size_t size);
void* bf_free(void* ptr);
unsigned long get_data_segment_free_space_size();
unsigned long get_data_segment_size();
