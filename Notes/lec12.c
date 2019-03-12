#include <stdio.h>  // printf
#include <stdlib.h>  // malloc, free
#include <assert.h>

int main(int argc, char* argv[])
{
  void* memory = malloc(480);
  assert(memory != NULL);
  printf("Yay!\n");
  free(memory);
  return 0;
}