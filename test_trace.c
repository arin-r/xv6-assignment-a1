// user_test.c
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  getpid();        // Example syscall
  sleep(10);       // Example syscall
  print_trace();   // Print the syscall counts
  exit();
}
