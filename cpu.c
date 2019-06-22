#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  // mar = Memory Address registersister, holds the memory address being written or writing
  // return value from ram specified by mar
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char mdr)
{
  // mdr = Memory Data registersister, holds the value to write or just read
  // write mdr in ram
  cpu->ram[index] = mdr;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  // Decrement SP
  cpu->registers[SP]--;
  // Copy the value in the given registers to the address pointed to by SP
  cpu_ram_write(cpu, cpu->registers[SP], val);
}

unsigned char cpu_pop(struct cpu *cpu)
{
  //Read last value from stack
  unsigned char val = cpu_ram_read(cpu, cpu->registers[SP]);
  // Increment SP
  cpu->registers[SP]++;
  return val;
}

/* Load the binary bytes from a .ls8 source file into a RAM array */

// void cpu_load(char *filename, struct cpu *cpu)
// {
//   FILE *fp = fopen(filename, "r");
//   if (fp == NULL)
//   {
//     fprintf(stderr, "ls8: Error in opening file: %s\n", filename);
//     exit(2);
//   }

//   char line[1024]; //hold individual lines in the file
//   int address = ADDR_PROGRAM_ENTRY;

//   while (fgets(line, sizeof(line), fp) != NULL)
//   {
//     char *endptr; // to keep track of non-numbers in the file
//     //converts str to number
//     unsigned char byte = strtoul(line, &endptr, 2);
//     //prevents unnecessary lines from being stored on ram
//     if (endptr == line)
//     {
//       continue;
//     }
//     cpu->ram[address++] = byte;
//   }
//   fclose(fp);
// }

void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  // Open the source file
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  // Read all the lines and store them in RAM
  while (fgets(line, sizeof line, fp) != NULL) {

    // Convert string to a number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);;

    // Ignore lines from which no numbers were read
    if (endchar == line) {
      continue;
    }

    // Store in ram
    cpu->ram[address++] = byte;
  }
}

/* Run the CPU */
void cpu_run(struct cpu *cpu)
{
  //True until we get a HLT instruction
  unsigned char *registers = cpu->registers;
  unsigned char *ram = cpu->ram;
  int running = 1;
  // unsigned char command, operand1, operand2;

  while (running)
  {
    unsigned char command = ram[cpu->PC];

    unsigned char operands = command >> 6;
    // operand1 = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    // operand2 = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    unsigned char operand1 = ram[(cpu->PC + 1) & 0xff];
    unsigned char operand2 = ram[(cpu->PC + 2) & 0xff];

    switch (command)
    {
    case LDI:
      // sets value of operand1 to the registers[operand1]
      registers[operand1] = operand2;
      break;

    case PRN:
      printf("%d\n", registers[operand1]);
      break;

    case HLT:
      // Set running to false to stop program
      running = 0;
      // Move the PC to the next instruction
      cpu->PC += 1;
      break;

    case MUL:
      registers[operand1] *= operand2;
      cpu->PC += 3;
      break;

    case ADD:
      registers[operand1] += operand2;
      cpu->PC += 3;
      break;

    case PUSH:
      cpu_push(cpu, registers[operand1]);
      cpu->PC += 2;
      break;

    case POP:
      // Copy the value from the address pointed to by SP to the given registers
      registers[operand1] = cpu_pop(cpu);
      cpu->PC += 2;
      break;

    case JMP:
      cpu->PC = registers[operand1] - operands - 1;
      break;

    case CMP:
      if (registers[operand1] == registers[operand2])
      {
        cpu->FL = 0b00000001;
      }
      // if op2 is greater set flag to 0b00000100
      else if (registers[operand1] < registers[operand2])
      {
        cpu->FL = 0b00000100;
      }
      // if op1 is greater set flag to 0b00000010
      else if (registers[operand1] > registers[operand2])
      {
        cpu->FL = 0b00000010;
      }
      break;

    case JEQ:
      if (cpu->FL == 00000001)
      {
        cpu->PC = registers[operand1] - operands - 1;
      }
      break;

    case JNE:
      if ((cpu->FL & 00000001) == 0)
      {
        cpu->PC = registers[operand1] - operands - 1;
      }
      break;

    default:
      printf("Danger Will Robinson! DANGER!\n");
      exit(1);
    }
    cpu->PC += operands + 1;
  }
}

/* Initialize a CPU struct */
// void cpu_init(struct cpu *cpu)
// {
//   // R0-R6 are cleared to 0
//   for (int i = 0; i < 6; i++)
//   {
//     cpu->registers[i] = 0;
//     cpu
//   }
//   // R7 is set to 0xF4
//   cpu->registers[7] = 0xF4;
//   // PC is cleared to 0
//   memset(cpu->registers, 0, sizeof(cpu->registers[SP]));
//   // RAM is cleared to 0
//   memset(cpu->ram, 0, sizeof(cpu->ram));
// }

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;


  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);


  cpu->registers[SP] = ADDR_EMPTY_STACK;
}