#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <string.h>

#include "vm.h"

void opcode_mov(VM *vm, struct _Instruction *instruction)
{
	union Template *regDest = (union Template *)instruction->regDest;
	union Template *regSrc = (union Template *)instruction->regSrc;
	
	if(instruction->isImm)
	{
		if(instruction->sign)
		{
			regDest->s += instruction->imm.s;
		}
		else
		{
			regDest->u += instruction->imm.u;
		}
	}
	else
	{
		//if(
	}
}

void opcode_add(VM *vm, struct _Instruction *instruction)
{
	union Template *regDest = (union Template *)instruction->regDest;
	union Template *regSrc = (union Template *)instruction->regSrc;
	
	if(instruction->isImm)
	{
		if(instruction->sign)
		{
			regDest->s += instruction->imm.s;
		}
		else
		{
			regDest->u += instruction->imm.u;
		}
	}
}

void opcode_sub(VM *vm, struct _Instruction *instruction)
{
	union Template *regDest = (union Template *)instruction->regDest;
	union Template *regSrc = (union Template *)instruction->regSrc;
	
	if(instruction->isImm)
	{
		if(instruction->sign)
		{
			regDest->s += instruction->imm.s;
		}
		else
		{
			regDest->u += instruction->imm.u;
		}
	}
}

void opcode_div(VM *vm, struct _Instruction *instruction)
{
	union Template *regDest = (union Template *)instruction->regDest;
	union Template *regSrc = (union Template *)instruction->regSrc;
	
	if(instruction->isImm)
	{
		if(instruction->sign)
		{
			regDest->s += instruction->imm.s;
		}
		else
		{
			regDest->u += instruction->imm.u;
		}
	}
}

void opcode_nop(VM *vm, struct _Instruction *instruction)
{
	return;
}

