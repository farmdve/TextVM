#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <string.h>

#include "vm.h"

#define opcode(opcode) void opcode_##opcode(VM *vm, struct _Instruction *instruction)

opcode(mov)
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

opcode(add)
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
		if(FLAG_CHECK(sum, i))
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

opcode(sub)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	unsigned int borrow = 0;
	unsigned int sum = 0, x, y,  x2, y2;
	unsigned int bits_set = 0;

	if(instruction->isImm)
		y = instruction->imm;
	else
		y = *regSrc;
		
	x = *regDest;
	x2 = x;
	y2 = y;		

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
		
	for(int i = 0; i < 32; i++)
	{
		if(FLAG_CHECK(sum, i))
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

opcode(div)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	//unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	
	// todo
	
	//todo modify EFLAGS
}

opcode(mul)
{
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	unsigned long long int sum = 0, x, y, x2, y2;
	
	if(instruction->isImm)
		return; // mul never has any immediate value
		
	y = *regSrc;
	
	
	x = vm->regs.EAX;
	
	//cheat
	sum = x * y;
	
	if(((sum >> 32) & 0xFFFFFFFF) == 0)
	{
		FLAG_CLEAR(&vm->regs.EFLAGS, CF);
		FLAG_CLEAR(&vm->regs.EFLAGS, OF);		
	}
	else
	{
		FLAG_SET(&vm->regs.EFLAGS, CF);
		FLAG_SET(&vm->regs.EFLAGS, OF);			
	}
	
	vm->regs.EDX = (sum >> 32);
	vm->regs.EAX = sum & 0xFFFFFFFF;

}

opcode(xor)
{
	unsigned int *regDest = (unsigned int *)instruction->regDest;
	unsigned int *regSrc = (unsigned int *)instruction->regSrc;
	unsigned int sum = 0, bits_set = 0;
	
	if(instruction->isImm)
		*regDest = *regDest ^ instruction->imm;
	else
		*regDest = *regDest ^ *regSrc;
	
	sum = *regDest;
	
	if(sum == 0)
		FLAG_SET(&vm->regs.EFLAGS, ZF);
	else
		FLAG_CLEAR(&vm->regs.EFLAGS, ZF);
			
	if(FLAG_CHECK(sum, SIGN_BIT))
		FLAG_SET(&vm->regs.EFLAGS, SF);
		
	for(int i = 0; i < 32; i++)
	{
		if(FLAG_CHECK(sum, i))
			bits_set++;
	}

	if(bits_set % 2)
		FLAG_SET(&vm->regs.EFLAGS, PF);
	else
		FLAG_CLEAR(&vm->regs.EFLAGS, PF);	
		
	FLAG_CLEAR(&vm->regs.EFLAGS, CF);
	FLAG_CLEAR(&vm->regs.EFLAGS, OF);
}

opcode(nop)
{
	return;
}

