#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <string.h>

#include "vm.h"

void opcode_mov(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	
	if(instruction->isImm)
	{
		*regDest += instruction->imm;
	}
	else
	{
		*regDest += *regSrc;
	}
}

void opcode_add(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	
	if(instruction->isImm)
	{
		*regDest += instruction->imm;
	}
	else
	{
		*regDest += *regSrc;
	}	
}

void opcode_sub(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	
	if(instruction->isImm)
	{
		*regDest -= instruction->imm;
	}
	else
	{
		*regDest -= *regSrc;
	}
}

void opcode_div(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	//unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	
	// todo
	
	//todo modify EFLAGS
}

void opcode_xor(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	
	if(instruction->isImm)
		*regDest = *regDest ^ instruction->imm;
	else
		*regDest = *regDest ^ *regSrc;
}

void opcode_nop(VM *vm, struct _Instruction *instruction)
{
	return;
}

