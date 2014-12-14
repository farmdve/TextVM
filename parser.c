#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


#include <ctype.h>

#include "vm.h"


struct instruction_table ins_table[] = {
	{MOV_INSTRUCTION, "mov", opcode_mov},
	{ADD_INSTRUCTION, "add", opcode_add},
	{SUB_INSTRUCTION, "sub", opcode_sub},
	{DIV_INSTRUCTION, "div", opcode_div},
	{XOR_INSTRUCTION, "xor", opcode_xor},	
	{0, 0, 0}
};

struct register_table reg_table[] = {
	{REG_EAX, "eax", 4, 0},
	{REG_ECX, "ecx", 4, 0},
	{REG_EDX, "edx", 4, 0},
	{REG_EBX, "ebx", 4, 0},
	{REG_ESP, "esp", 4, 0},
	{REG_EBP, "ebp", 4, 0},
	{REG_ESI, "esi", 4, 0},
	{REG_EDI, "edi", 4, 0},
	{REG_AX, "ax", 2, 0},
	{REG_CX, "cx", 2, 0},
	{REG_DX, "dx", 2, 0},
	{REG_BX, "bx", 2, 0},
	{REG_BP, "bp", 2, 0},
	{REG_SI, "si", 2, 0},
	{REG_DI, "di", 2, 0},
	{REG_AL, "al", 2, 0},
	{REG_CL, "cl", 2, 0},
	{REG_DL, "dl", 2, 0},
	{REG_BL, "bl", 2, 0},
	{REG_AH, "ah", 2, 0},
	{REG_CH, "ch", 2, 0},
	{REG_DH, "dh", 2, 0},
	{REG_BH, "bh", 2, 0},	
	{0, 0, 0, 0}
};

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
	
	struct instruction_table *pins_table;
	for(pins_table = ins_table; pins_table->ins; pins_table++)
	{
		if(strcasecmp(temp, pins_table->ins) == 0)
		{
			type = pins_table->type;
			vm->instruction[instr].type = pins_table->type;
			vm->instruction[instr].handler = pins_table->opcode;
			
			break;
		}		
	}
	
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
		{
			if(strstr(buf, "dword"))
				vm->instruction[instr].size = 4;
			else if(strstr(buf, "word"))
				vm->instruction[instr].size = 2;
			else if(strstr(buf, "byte"))
				vm->instruction[instr].size = 1;
			else
				vm->instruction[instr].size = 4;
				
			vm->instruction[instr].isPointerDest = true;
		}

		if(strcasecmp(buf, "eax") == 0)
		{
			regmod = REG_EAX;
			vm->instruction[instr].regDest = &vm->regs.EAX;
		}	
		else if(strcasecmp(buf, "ecx") == 0)
		{
			regmod = REG_ECX;
			vm->instruction[instr].regDest = &vm->regs.ECX;
		}		
		else if(strcasecmp(buf, "edx") == 0)
		{
			regmod = REG_EDX;
			vm->instruction[instr].regDest = &vm->regs.EDX;
		}	
		else if(strcasecmp(buf, "ebx") == 0)
		{
			regmod = REG_EBX;
			vm->instruction[instr].regDest = &vm->regs.EBX;
		}		
		else if(strcasecmp(buf, "esp") == 0)
		{
			regmod = REG_ESP;
			vm->instruction[instr].regDest = &vm->regs.ESP;
		}
		else if(strcasecmp(buf, "ebp") == 0)
		{
			regmod = REG_EBP;
			vm->instruction[instr].regDest = &vm->regs.EBP;
		}
		else if(strcasecmp(buf, "esi") == 0)
		{
			regmod = REG_ESI;
			vm->instruction[instr].regDest = &vm->regs.ESI;
		}
		else if(strcasecmp(buf, "edi") == 0)
		{
			regmod = REG_EDI;
			vm->instruction[instr].regDest = &vm->regs.EDI;
		}
		
		if(strcasecmp(buf, "ax") == 0)
			regmod = REG_AX;
			
		if(strcasecmp(buf, "cx") == 0)
			regmod = REG_CX;	
			
		if(strcasecmp(buf, "dx") == 0)
			regmod = REG_DX;	
			
		if(strcasecmp(buf, "bx") == 0)
			regmod = REG_BX;			

		if(strcasecmp(buf, "bp") == 0)
			regmod = REG_BP;

		if(strcasecmp(buf, "si") == 0)
			regmod = REG_SI;

		if(strcasecmp(buf, "di") == 0)
			regmod = REG_DI;			

		if(strcasecmp(buf, "al") == 0)
			regmod = REG_AL;	
			
		if(strcasecmp(buf, "cl") == 0)
			regmod = REG_CL;	
			
		if(strcasecmp(buf, "dl") == 0)
			regmod = REG_DL;	
			
		if(strcasecmp(buf, "bl") == 0)
			regmod = REG_BL;

		if(strcasecmp(buf, "ah") == 0)
			regmod = REG_AH;	
			
		if(strcasecmp(buf, "ch") == 0)
			regmod = REG_CH;	
			
		if(strcasecmp(buf, "dh") == 0)
			regmod = REG_DH;	
			
		if(strcasecmp(buf, "bh") == 0)
			regmod = REG_BH;
			
		vm->instruction[instr].regmodDest = regmod;	
	}
	else if(pass == 2)
	{
		if(strstr(buf, "["))
		{
			if(strstr(buf, "dword"))
				vm->instruction[instr].size = 4;
			else if(strstr(buf, "word"))
				vm->instruction[instr].size = 2;
			else if(strstr(buf, "byte"))
				vm->instruction[instr].size = 1;
			else
				vm->instruction[instr].size = 4;
				
			vm->instruction[instr].isPointerSrc = true;
		}
			

		if(strcasecmp(buf, "eax") == 0)
		{
			regmod = REG_EAX;
			vm->instruction[instr].regSrc = &vm->regs.EAX;
		}
			
		if(strcasecmp(buf, "ecx") == 0)
		{
			regmod = REG_ECX;
			vm->instruction[instr].regSrc = &vm->regs.ECX;
		}
			
		if(strcasecmp(buf, "edx") == 0)
		{
			regmod = REG_EDX;
			vm->instruction[instr].regSrc = &vm->regs.EDX;
		}
			
		if(strcasecmp(buf, "ebx") == 0)
		{
			regmod = REG_EBX;
			vm->instruction[instr].regSrc = &vm->regs.EBX;
		}
			
		if(strcasecmp(buf, "esp") == 0)
		{
			regmod = REG_ESP;
			vm->instruction[instr].regSrc = &vm->regs.ESP;
		}

		if(strcasecmp(buf, "ebp") == 0)
		{
			regmod = REG_EBP;
			vm->instruction[instr].regSrc = &vm->regs.EBP;
		}

		if(strcasecmp(buf, "esi") == 0)
		{
			regmod = REG_ESI;
			vm->instruction[instr].regSrc = &vm->regs.ESI;
		}

		if(strcasecmp(buf, "edi") == 0)
		{
			regmod = REG_EDI;
			vm->instruction[instr].regSrc = &vm->regs.EDI;
		}
		
		if(strcasecmp(buf, "ax") == 0)
			regmod = REG_AX;	
			
		if(strcasecmp(buf, "cx") == 0)
			regmod = REG_CX;	
			
		if(strcasecmp(buf, "dx") == 0)
			regmod = REG_DX;	
			
		if(strcasecmp(buf, "bx") == 0)
			regmod = REG_BX;			

		if(strcasecmp(buf, "bp") == 0)
			regmod = REG_BP;

		if(strcasecmp(buf, "si") == 0)
			regmod = REG_SI;

		if(strcasecmp(buf, "di") == 0)
			regmod = REG_DI;			

		if(strcasecmp(buf, "al") == 0)
			regmod = REG_AL;	
			
		if(strcasecmp(buf, "cl") == 0)
			regmod = REG_CL;	
			
		if(strcasecmp(buf, "dl") == 0)
			regmod = REG_DL;	
			
		if(strcasecmp(buf, "bl") == 0)
			regmod = REG_BL;

		if(strcasecmp(buf, "ah") == 0)
			regmod = REG_AH;	
			
		if(strcasecmp(buf, "ch") == 0)
			regmod = REG_CH;	
			
		if(strcasecmp(buf, "dh") == 0)
			regmod = REG_DH;	
			
		if(strcasecmp(buf, "bh") == 0)
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
		unsigned int imm = 0;
		sscanf(buf, "%x", &imm);
		vm->instruction[instr].imm = imm;
			
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

int countInstructions(VM *vm, char *ins)
{
	char *pch;
	int c = 0;
	
	pch = strtok(ins, "\n");
	while(pch)
	{
		c++;
		pch = strtok(NULL, "\n");
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

	vm->nInstructions = countInstructions(vm, buff1);
	
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
		instruction[i] = malloc(strlen(pch) + 1);
		strcpy(instruction[i], pch);		
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