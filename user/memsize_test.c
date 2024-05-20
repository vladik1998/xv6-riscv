#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
  printf("This process is using %d bytes\n", memsize());
  printf("Allocating 20,000 bytes using malloc...\n");
  void *allocated = malloc(20000);
  printf("This process is using %d bytes\n", memsize());
  printf("Freeing 20,000 bytes using free...\n");
  free(allocated);
  printf("This process is using %d bytes\n", memsize());
  exit(0, 0);
}
