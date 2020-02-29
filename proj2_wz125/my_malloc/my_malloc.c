#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include"my_malloc.h"
#include<assert.h>
#include<limits.h>

void* bf_malloc(size_t size, int noLock, metadata** head){
  size_t realsize = size + sizeof(metadata);
  metadata* alloc_add;
  metadata** current = head;
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
  if(noLock == 1){
    pthread_mutex_lock(&lock);
    alloc_add = sbrk(realsize);
    pthread_mutex_unlock(&lock);
    }
  else if (noLock == 0){
    alloc_add = sbrk(realsize);
  }
  
  alloc_add->block_size = size;
  alloc_add->next = NULL;
  return alloc_add + 1;
}

void add_new(metadata* ptr_block, metadata** head){
  metadata **current = head;
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

void merge(metadata* ptr_block, metadata** head){
  add_new(ptr_block, head);
  metadata* ptr_end = (metadata*)((size_t)ptr_block + ptr_block->block_size + sizeof(metadata));
  metadata * check_end;
  metadata * check_next;
  if(ptr_end == ptr_block->next && ptr_block->next != NULL){
    merge_ptr(ptr_block, ptr_end);
    }
  metadata * check =*head;
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

void* bf_free(void* ptr, metadata** head){
  //return ptr to the address of metadata block
  metadata* ptr_block =(metadata*)ptr - 1;
  merge(ptr_block, head);
  return NULL;
}
void* ts_malloc_lock(size_t size){
  pthread_mutex_lock(&lock);
  void *ptr =bf_malloc(size,0,&freed_head_lock);
  pthread_mutex_unlock(&lock);
  return ptr;
}
void ts_free_lock(void* ptr){
  pthread_mutex_lock(&lock);
  bf_free(ptr, &freed_head_lock);
  pthread_mutex_unlock(&lock);
}
void* ts_malloc_nolock(size_t size){
  //int flag = 1;
  void *ptr =bf_malloc(size, 1, &freed_head_nolock);
  return ptr;
}
void ts_free_nolock(void* ptr){
  bf_free(ptr, &freed_head_nolock);
}

