#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <string.h>
#include <ctype.h>

#include "vm.h"

int INS_op(VM *vm, char *buf, int instr)
{
	int type = -1;
	size_t len = 0;
	char temp[50], *tmp2 = buf;
	
	strcpy(temp, buf);
	while(*(tmp2++))
	{
		len++;
		if(*tmp2 == ' ')
		{
			temp[len] = '\0';
			break;
		}		
	}
	
	if(strcmp(temp, "mov") == 0)
	{
		type = MOV_INSTRUCTION;
		vm->instruction[instr].handler = &opcode_mov;
	}
		
	if(strcmp(temp, "add") == 0)
	{
		type = ADD_INSTRUCTION;
		vm->instruction[instr].handler = &opcode_add;
	}
		
	if(strcmp(temp, "sub") == 0)
	{
		type = SUB_INSTRUCTION;
		vm->instruction[instr].handler = &opcode_sub;
	}
		
	if(strcmp(temp, "div") == 0)
	{
		type = DIV_INSTRUCTION;
		vm->instruction[instr].handler = &opcode_div;
	}

	if(strcmp(temp, "mul") == 0)
	{
		type = MUL_INSTRUCTION;
		vm->instruction[instr].handler = &opcode_mov;
	}
	
	if(strcmp(temp, "nop") == 0)
	{
		type = NOP_INSTRUCTION;
		vm->instruction[instr].handler = &opcode_nop;
	}	

	
	vm->instruction[instr].type = type;
	
	if(type < 0) // unknown instruction
		vm->instruction[instr].handler = &opcode_nop;
	
	return len;
}

int INS_Split(VM *vm, char *buf, int pass, int instr)
{
	int regmod = -1;
	
	if(pass == 1)
	{
		if(strstr(buf, "["))
			vm->instruction[instr].isPointerDest = true;

		if(strcmp(buf, "eax") == 0)
		{
			regmod = REG_EAX;
			vm->instruction[instr].regDest = &vm->regs.EAX;
		}
			
		if(strcmp(buf, "ecx") == 0)
		{
			regmod = REG_ECX;
			vm->instruction[instr].regDest = &vm->regs.ECX;
		}
			
		if(strcmp(buf, "edx") == 0)
		{
			regmod = REG_EDX;
			vm->instruction[instr].regDest = &vm->regs.EDX;
		}
			
		if(strcmp(buf, "ebx") == 0)
		{
			regmod = REG_EBX;
			vm->instruction[instr].regDest = &vm->regs.EBX;
		}
			
		if(strcmp(buf, "esp") == 0)
		{
			regmod = REG_ESP;
			vm->instruction[instr].regDest = &vm->regs.ESP;
		}

		if(strcmp(buf, "ebp") == 0)
		{
			regmod = REG_EBP;
			vm->instruction[instr].regDest = &vm->regs.EBP;
		}

		if(strcmp(buf, "esi") == 0)
		{
			regmod = REG_ESI;
			vm->instruction[instr].regDest = &vm->regs.ESI;
		}

		if(strcmp(buf, "edi") == 0)
		{
			regmod = REG_EDI;
			vm->instruction[instr].regDest = &vm->regs.EDI;
		}
			
		if(strcmp(buf, "ax") == 0)
			regmod = REG_AX;	
			
		if(strcmp(buf, "cx") == 0)
			regmod = REG_CX;	
			
		if(strcmp(buf, "dx") == 0)
			regmod = REG_DX;	
			
		if(strcmp(buf, "bx") == 0)
			regmod = REG_BX;			

		if(strcmp(buf, "bp") == 0)
			regmod = REG_BP;

		if(strcmp(buf, "si") == 0)
			regmod = REG_SI;

		if(strcmp(buf, "di") == 0)
			regmod = REG_DI;			

		if(strcmp(buf, "al") == 0)
			regmod = REG_AL;	
			
		if(strcmp(buf, "cl") == 0)
			regmod = REG_CL;	
			
		if(strcmp(buf, "dl") == 0)
			regmod = REG_DL;	
			
		if(strcmp(buf, "bl") == 0)
			regmod = REG_BL;

		if(strcmp(buf, "ah") == 0)
			regmod = REG_AH;	
			
		if(strcmp(buf, "ch") == 0)
			regmod = REG_CH;	
			
		if(strcmp(buf, "dh") == 0)
			regmod = REG_DH;	
			
		if(strcmp(buf, "bh") == 0)
			regmod = REG_BH;
			
		vm->instruction[instr].regmodDest = regmod;	
	}
	else if(pass == 2)
	{
		if(strstr(buf, "["))
			vm->instruction[instr].isPointerSrc = true;
			

		if(strcmp(buf, "eax") == 0)
		{
			regmod = REG_EAX;
			vm->instruction[instr].regSrc = &vm->regs.EAX;
		}
			
		if(strcmp(buf, "ecx") == 0)
		{
			regmod = REG_ECX;
			vm->instruction[instr].regSrc = &vm->regs.ECX;
		}
			
		if(strcmp(buf, "edx") == 0)
		{
			regmod = REG_EDX;
			vm->instruction[instr].regSrc = &vm->regs.EDX;
		}
			
		if(strcmp(buf, "ebx") == 0)
		{
			regmod = REG_EBX;
			vm->instruction[instr].regSrc = &vm->regs.EBX;
		}
			
		if(strcmp(buf, "esp") == 0)
		{
			regmod = REG_ESP;
			vm->instruction[instr].regSrc = &vm->regs.ESP;
		}

		if(strcmp(buf, "ebp") == 0)
		{
			regmod = REG_EBP;
			vm->instruction[instr].regSrc = &vm->regs.EBP;
		}

		if(strcmp(buf, "esi") == 0)
		{
			regmod = REG_ESI;
			vm->instruction[instr].regSrc = &vm->regs.ESI;
		}

		if(strcmp(buf, "edi") == 0)
		{
			regmod = REG_EDI;
			vm->instruction[instr].regSrc = &vm->regs.EDI;
		}
		
		if(strcmp(buf, "ax") == 0)
			regmod = REG_AX;	
			
		if(strcmp(buf, "cx") == 0)
			regmod = REG_CX;	
			
		if(strcmp(buf, "dx") == 0)
			regmod = REG_DX;	
			
		if(strcmp(buf, "bx") == 0)
			regmod = REG_BX;			

		if(strcmp(buf, "bp") == 0)
			regmod = REG_BP;

		if(strcmp(buf, "si") == 0)
			regmod = REG_SI;

		if(strcmp(buf, "di") == 0)
			regmod = REG_DI;			

		if(strcmp(buf, "al") == 0)
			regmod = REG_AL;	
			
		if(strcmp(buf, "cl") == 0)
			regmod = REG_CL;	
			
		if(strcmp(buf, "dl") == 0)
			regmod = REG_DL;	
			
		if(strcmp(buf, "bl") == 0)
			regmod = REG_BL;

		if(strcmp(buf, "ah") == 0)
			regmod = REG_AH;	
			
		if(strcmp(buf, "ch") == 0)
			regmod = REG_CH;	
			
		if(strcmp(buf, "dh") == 0)
			regmod = REG_DH;	
			
		if(strcmp(buf, "bh") == 0)
			regmod = REG_BH;
			
		vm->instruction[instr].regmodSrc = regmod;
	}	
	
	if(regmod > 0)
	{
		if(pass == 2)
			vm->instruction[instr].isRegisterSrc = true;
		if(pass == 1)
			vm->instruction[instr].isRegisterDest = true;
	}
	else if(regmod < 0 && pass == 2)
	{
		int sign = 0;
		sscanf(buf, "%x", &sign);
		if(sign < 0)
		{
			vm->instruction[instr].sign = true;
			vm->instruction[instr].imm.s = strtol(buf, NULL, 16);
		}
		else
		{
			vm->instruction[instr].sign = false;
			vm->instruction[instr].imm.u = strtoul(buf, NULL, 16);
		}
			
		vm->instruction[instr].isImm = true;
	
	}
	
	return 0;
	
}

int countComma(const char *buf)
{
	const char *s = buf;
	int c = 0;
	
	while(*(s++))
	{
		if(*s == ',')
			c++;
	}
	
	return c;
}

int INS_parse(VM *vm, const char *disasm)
{
	char buff1[100], buff2[100], **instruction;
	char *pch;
	int i = 0;
	
	strcpy(buff1, disasm); //len check todo
	strcpy(buff2, disasm);

	pch = strtok(buff1, "\n");
	while(pch)
	{
		vm->nInstructions++;
		pch = strtok(NULL, "\n");
	}
	
	if(vm->nInstructions <= 0)
		return -1;
		
	printf("Number of instructions: %d\n", vm->nInstructions);
		
	vm->instruction = calloc(vm->nInstructions, sizeof(struct _Instruction));
	if(!vm->instruction)
		return -2;

	for(int k = 0; k < vm->nInstructions; k++)
	{
		memset(&vm->instruction[k], 0, sizeof(struct _Instruction));		
	}
	
	instruction = calloc(vm->nInstructions, sizeof(char *));

	pch = strtok(buff2, "\n");
	while(pch)
	{
		char buf[100];
		int j = 0;
		for(j = 0; j < strlen(pch); j++)
		{
			
			buf[j] = (char)tolower((int)(pch[j]));
		}
		buf[j] = '\0';	
		instruction[i] = malloc(strlen(pch) + 1);
		strcpy(instruction[i], buf);		
		i++;
		pch = strtok(NULL, "\n");
	}
	
	for(int i = 0; i < vm->nInstructions; i++)
	{
		char buf[100];
		strcpy(buf, instruction[i]);	
		
		int c = countComma(buf);
		pch = strtok(buf, ",");
		while(pch)
		{
			INS_op(vm, pch, i);
			pch = strtok(NULL, ",");
			
			break;
		}		
	}
	
	for(int i = 0; i < vm->nInstructions; i++)
	{
		char buf[100], *temp = buf;
		strcpy(buf, instruction[i]);
		
		while(*(temp++))
		{
			if(*temp == ' ')
			{
				*(temp++);
				break;
			}
		}
		
		int c = countComma(buf), pass = 1;
		pch = strtok(temp, ",");
		while(pch)
		{
			
			int res = INS_Split(vm, pch, pass, i); // sanity checks todo
			pch = strtok(NULL, ",");
			pass++;
		}
		
		free(instruction[i]);
	}	

	return 0;
}