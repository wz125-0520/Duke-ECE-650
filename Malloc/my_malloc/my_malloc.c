#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include"my_malloc.h"
#include<assert.h>
#include<limits.h>



void* bf_malloc(size_t size){
  size_t realsize = size + sizeof(metadata);
  metadata* alloc_add;
  metadata** current = &freed_head;
  metadata** act = NULL;
  size_t sub = ULONG_MAX;
  //metadata* current = freed_head
  while((*current)!= NULL){
    if((*current)->block_size == size){
      alloc_add = (*current);
      (*current) = (*current)->next;
      alloc_add->block_size = size;	
      return alloc_add + 1;
    }
    if((*current)->block_size > size + sizeof(metadata) && (*current)->block_size - size < sub){
      sub = (*current)->block_size - size;
      act = current;
    }
    current = &(*current)->next;
  }
  if(act != NULL){
    alloc_add = *act;
    metadata* temp = (metadata*)((size_t)(*act) + realsize);
    temp->block_size = sub - sizeof(metadata);
    temp->next = (*act)->next;
    (*act) = temp;
    alloc_add->block_size = size;
    return alloc_add +1;
  }
  alloc_add = sbrk(realsize);
  if((void*)alloc_add == (void*)-1){
    printf("sbrk failed\n");
    exit(EXIT_FAILURE);
  }
  space += realsize;
  alloc_add->block_size = size;
  alloc_add->next = NULL;
  return alloc_add + 1;
}
void* ff_malloc(size_t size){
  size_t realsize = size + sizeof(metadata);
  metadata* alloc_add;
  metadata** current = &freed_head;
  while((*current) != NULL){
    if((*current)->block_size  == size){
	alloc_add = (*current);
    	(*current) = (*current)->next;
	alloc_add->block_size = size;	
	return alloc_add + 1;
    }
   if((*current)->block_size > realsize){
    //use the remain part of the block
            alloc_add = (*current);
	    metadata * temp = (metadata*)((size_t)(*current) + realsize);
	    temp->block_size =  (*current)->block_size - realsize;
	    temp->next = (*current)->next;
	    *current = temp;
	    alloc_add->block_size = size;	
            return alloc_add + 1;
      
    }
     current = &(*current)->next;
  }                                                      
  alloc_add = sbrk(realsize);
  if((void*)alloc_add == (void*)-1){
    printf("sbrk failed\n");
    exit(EXIT_FAILURE);
  }
  space += realsize;
  alloc_add->block_size = size;
  alloc_add->next = NULL;
  //printf("malloc_address: %zu\n",(size_t)(alloc_add+1));
  return alloc_add + 1;
}

void malloc_freed( metadata** current, size_t realsize){

  if((*current)->block_size + sizeof(metadata) == realsize){
    (*current) = (*current)->next;
  }
  else{
    //use the remain part of the block
    metadata * temp = (metadata*)((size_t)(*current) + realsize);
    temp->block_size =  (*current)->block_size - realsize;
    temp->next = (*current)->next;
    *current = temp;
  }

}

void* ff_free(void* ptr){
  //return ptr to the address of metadata block
  metadata* ptr_block =(metadata*)ptr - 1;
  merge(ptr_block);
  return NULL;
}
void* bf_free(void* ptr){
  ff_free(ptr);
  return NULL;
}
void add_new(metadata* ptr_block){
  metadata **current = &freed_head;
  while((*current) < ptr_block && (*current) != NULL){
      current = &(*current)->next;
  }
  metadata* nextb = (*current);
  ptr_block->next = nextb;
  (*current) = ptr_block;
}
void merge_ptr(metadata* ptr_block, metadata* ptr_end){
 ptr_block->block_size += ptr_end->block_size + sizeof(metadata);
 ptr_block->next = ptr_end->next;
}
void merge(metadata* ptr_block){
  add_new(ptr_block);
  metadata* ptr_end = (metadata*)((size_t)ptr_block + ptr_block->block_size + sizeof(metadata));
  metadata * check_end;
  metadata * check_next;
  if(ptr_end == ptr_block->next && ptr_block->next != NULL){
    merge_ptr(ptr_block, ptr_end);
    }
  metadata * check = freed_head;
  while(check->next != NULL && check < ptr_block){
    check_next = check->next;
    check_end = (metadata*)((char*)(check) + check->block_size + sizeof(metadata));
    if(check_end == check_next){
      check->block_size += check_end->block_size + sizeof(metadata);
      check->next = check_end->next;
      return;
    }
    check = check->next;
  } 
}

unsigned long get_data_segment_size(){
  return space;
}
unsigned long get_data_segment_free_space_size(){
  metadata* current = freed_head;
  unsigned long free_space = 0;
  int count = 0;
  while(current != NULL){
    count++;
    free_space += current->block_size;
    current = current->next;
  }
  //printf("%d", count);
  return free_space + count * sizeof(metadata) ;
}
