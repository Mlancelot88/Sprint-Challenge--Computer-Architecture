#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
// int main(int argc, char **argv)
// {
//   struct cpu cpu;
//   if (argc != 2)
//   {
//     fprintf(stderr, "Usage: ls8 filename\n");
//     exit(1);
//   }

//   char *filename = argv[1];

//   cpu_init(&cpu);
//   cpu_load(&cpu, filename);
//   cpu_run(&cpu);

//   return 0;
// }

/**
 * Main
 */

int main(int argc, char **argv)
{
  struct cpu cpu;

  if (argc != 2) {
    fprintf(stderr, "usage: ls8 file.ls8\n");
    return 1;
  }

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}