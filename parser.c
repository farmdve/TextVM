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
	struct register_table *pReg_table;
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

		for(pReg_table = reg_table; pReg_table->reg; pReg_table++)
		{
			if(strcasecmp(buf, pReg_table->reg) == 0)
			{
				vm->instruction[instr].regDest = pReg_table->pReg;
				vm->instruction[instr].regmodDest = pReg_table->type;
				break;
			}		
		}
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
		
		for(pReg_table = reg_table; pReg_table->reg; pReg_table++)
		{
			if(strcasecmp(buf, pReg_table->reg) == 0)
			{
				vm->instruction[instr].regSrc = pReg_table->pReg;
				vm->instruction[instr].regmodSrc = pReg_table->type;
				break;
			}		
		}
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
	
	
	for(int n = 0; n < sizeof(ins_table)/sizeof(struct instruction_table); n++)
	{
		if(strcasecmp(reg_table[n].reg, "eax") == 0)
		{
			reg_table[n].pReg = &vm->regs.EAX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "ecx") == 0)
		{
			reg_table[n].pReg = &vm->regs.ECX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "edx") == 0)
		{
			reg_table[n].pReg = &vm->regs.EDX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "ebx") == 0)
		{
			reg_table[n].pReg = &vm->regs.EBX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "esp") == 0)
		{
			reg_table[n].pReg = &vm->regs.ESP;
		}

		else if(strcasecmp(reg_table[n].reg, "ebp") == 0)
		{
			reg_table[n].pReg = &vm->regs.EBP;
		}

		else if(strcasecmp(reg_table[n].reg, "esi") == 0)
		{
			reg_table[n].pReg = &vm->regs.ESI;
		}

		else if(strcasecmp(reg_table[n].reg, "edi") == 0)
		{
			reg_table[n].pReg = &vm->regs.EDI;
		}
		
		else if(strcasecmp(reg_table[n].reg, "ax") == 0)
		{
			reg_table[n].pReg = &vm->regs.EAX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "cx") == 0)
		{
			reg_table[n].pReg = &vm->regs.ECX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "dx") == 0)
		{
			reg_table[n].pReg = &vm->regs.EDX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "bx") == 0)
		{
			reg_table[n].pReg = &vm->regs.EBX;
		}

		else if(strcasecmp(reg_table[n].reg, "bp") == 0)
		{
			reg_table[n].pReg = &vm->regs.EBP;
		}

		else if(strcasecmp(reg_table[n].reg, "si") == 0)
		{
			reg_table[n].pReg = &vm->regs.ESI;
		}

		else if(strcasecmp(reg_table[n].reg, "di") == 0)
		{
			reg_table[n].pReg = &vm->regs.EDI;
		}

		else if(strcasecmp(reg_table[n].reg, "al") == 0)
		{
			reg_table[n].pReg = &vm->regs.EAX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "cl") == 0)
		{
			reg_table[n].pReg = &vm->regs.ECX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "dl") == 0)
		{
			reg_table[n].pReg = &vm->regs.EDX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "bl") == 0)
		{
			reg_table[n].pReg = &vm->regs.EBX;
		}

		else if(strcasecmp(reg_table[n].reg, "ah") == 0)
		{
			reg_table[n].pReg = &vm->regs.EAX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "ch") == 0)
		{
			reg_table[n].pReg = &vm->regs.ECX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "dh") == 0)
		{
			reg_table[n].pReg = &vm->regs.EDX;
		}
			
		else if(strcasecmp(reg_table[n].reg, "bh") == 0)
		{
			reg_table[n].pReg = &vm->regs.EBX;
		}
	}	

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