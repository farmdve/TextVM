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
	unsigned int sum = 0, x, y, x2, y2;
	unsigned int bits_set = 0;
	
	if(instruction->isImm)
		y = instruction->imm;
	else
		y = *regSrc;
		
	x = *regDest;
	x2 = x;
	y2 = y;
	
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
		
	for(int i = 0; i < 32; i++)
	{
		if(FLAG_CHECK(sum, i)
			bits_set++;
	}
	
	if(bits_set % 2)
		FLAG_SET(&vm->regs.EFLAGS, PF);
	else
		FLAG_CLEAR(&vm->regs.EFLAGS, PF);
	
	bool sign = FLAG_CHECK(sum, SIGN_BIT);
	if(sign)
	{
		if(!FLAG_CHECK(x2, SIGN_BIT) && !FLAG_CHECK(y2, SIGN_BIT))
			FLAG_SET(&vm->regs.EFLAGS, OF);
		FLAG_SET(&vm->regs.EFLAGS, SF);	
	}
	else
	{
		if(FLAG_CHECK(x2, SIGN_BIT) && FLAG_CHECK(y2, SIGN_BIT))
			FLAG_SET(&vm->regs.EFLAGS, OF);
	}
			
	*regDest = sum;
}

void opcode_sub(VM *vm, struct _Instruction *instruction)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	unsigned int borrow = 0;
	unsigned int sum = 0, x, y;

	if(instruction->isImm)
		y = instruction->imm;
	else
		y = *regSrc;
		
	x = *regDest;
		

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

