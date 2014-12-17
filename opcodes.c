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
		*regDest = instruction->imm;	
	}
	else
	{
		*regDest = *regSrc;
	}
}

void opcode_add(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	unsigned int carry = 0;
	unsigned int sum = 0, x, y;
	
	if(instruction->isImm)
	{
		x = *regDest;
		y = instruction->imm;

		sum = x ^ y;
		carry = x & y;
		if(carry > 0)
			FLAG_SET(&vm->regs.EFLAGS, CF);
		
		while (carry != 0)
		{
			carry = carry << 1;
			x = sum;
			y = carry;
			sum = x ^ y;
			carry = x & y;
		}
		if(sum == 0)
			FLAG_SET(&vm->regs.EFLAGS, ZF);
		else
			FLAG_CLEAR(&vm->regs.EFLAGS, ZF);
			
		if(FLAG_CHECK(sum, SIGN_BIT))
			FLAG_SET(&vm->regs.EFLAGS, SF);			
			
		*regDest = sum;
	}
	else
	{
		//*regDest += *regSrc;
	}	
}

void opcode_sub(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	unsigned int borrow = 0;
	unsigned int sum = 0, x, y;

	if(instruction->isImm)
	{
		x = *regDest;
		y = instruction->imm;

		sum = x ^ -y;
		borrow = x & -y;
		if(borrow > 0)
			FLAG_SET(&vm->regs.EFLAGS, CF);
		
		while (borrow != 0)
		{
			borrow = borrow << 1;
			x = sum;
			y = borrow;
			sum = x ^ y;
			borrow = x & y;
		}
		if(sum == 0)
			FLAG_SET(&vm->regs.EFLAGS, ZF);
		else
			FLAG_CLEAR(&vm->regs.EFLAGS, ZF);
			
		if(FLAG_CHECK(sum, SIGN_BIT))
			FLAG_SET(&vm->regs.EFLAGS, SF);
			
		*regDest = sum;
	}
	else
	{
		//*regDest -= *regSrc;
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

