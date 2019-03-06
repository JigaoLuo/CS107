#include <stdio.h>
#include <string.h> // memcpy
#include <stdint.h>

void swap(void* , void* , int );

void swap(void* vp1, void* vp2, int size) 
{
  // void temp = *vp1;  absolutely wrong
  // 1. void is not a Data Type
  // 2. void* can not be de-referenced, the maschine doesn't know how many bytes to go out and embrace as part of the identification process.
  char buffer[size];
  printf("During Swapping\nFirst void*: %p\nSecond void*: %p\n", vp1, vp2);
  memcpy(buffer, vp1, size);
  for (int i = 0; i < size; i++) printf("buffer[%d]: %d\n", i, buffer[i]);
  memcpy(vp1, vp2, size);
  memcpy(vp2, buffer, size);
}

int main()
{
  // int i = 1; 
  // short s = 2;
  // printf("Before Swapping\nFirst integer = %d at %p\nSecond short = %hi at %p\n", i, (void*)&i, s, (void*)&s);
  // swap(&i, &s, sizeof(short));
  // printf("After Swapping\nFirst integer = %d at %p\nSecond short = %hi at %p\n", i, (void*)&i, s, (void*)&s);

  uint64_t i = 1; 
  uint32_t s = 2;
  printf("Before Swapping\nFirst uint64_t = %ld at %p\nSecond uint32_t = %hi at %p\n", i, (void*)&i, s, (void*)&s);
  swap(&i, &s, sizeof(uint32_t));
}