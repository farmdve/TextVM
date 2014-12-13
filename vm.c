#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <string.h>

#include "vm.h"

const char *inss = "mov eax,FFFFFF85\nadd eax,2";

VM *VM_Init()
{
	VM *vm = calloc(1, sizeof(VM));
	if(!vm)
		return NULL;
		
	memset(&vm->regs, 0, sizeof(VMRegs));
		
	vm->nInstructions = 0;
	
	return vm;
}

void VM_Exit(VM *vm)
{
	if(vm)
	{
		if(vm->instruction)
		{
			for(int i = 0; i < vm->nInstructions; i++)
				free(&vm->instruction[i]);		
			free(vm->instruction);
		}
		free(vm);
	}
}

void VM_Parse(VM *vm, const char *disasm)
{
	INS_parse(vm, disasm);
}


void VM_Execute(VM *vm)
{
	for(int instr = 0; instr < vm->nInstructions; instr++)
	{
		vm->instruction[instr].handler(vm, &vm->instruction[instr]);
	}
	
	printf("EAX %08x\n", vm->regs.EAX);
	printf("ECX %08x\n", vm->regs.ECX);
	printf("EDX %08x\n", vm->regs.EDX);
	printf("EBX %08x\n", vm->regs.EBX);
	printf("ESP %08x\n", vm->regs.ESP);
	printf("EBP %08x\n", vm->regs.EBP);
}

int main(int argc, char **argv)
{
	VM *vm;
	vm = VM_Init();
	if(!vm)
		return 0;

	VM_Parse(vm, inss);
	
	VM_Execute(vm);
	
	VM_Exit(vm);
	
	return 0;
}