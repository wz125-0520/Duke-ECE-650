#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>


typedef struct meta{
  size_t block_size;
  struct meta* next;
}metadata;

metadata* freed_head_lock = NULL;
__thread metadata* freed_head_nolock = NULL;

//unsigned long space = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void* ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);
//void merge( metadata* ptr_block, metadata* head);
//void add_new(metadata* ptr_block, metadata* head);
void* ts_malloc_nolock(size_t size);
void ts_free_nolock(void* ptr);

//unsigned long get_data_segment_free_space_size();
//unsigned long get_data_segment_size();
