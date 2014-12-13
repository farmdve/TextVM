#ifndef VM_H
#define VM_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <string.h>

enum {
	NOP_INSTRUCTION = 0,
	MOV_INSTRUCTION = 1,
	ADD_INSTRUCTION = 2,
	SUB_INSTRUCTION = 3,
	MUL_INSTRUCTION = 4,
	DIV_INSTRUCTION = 5
};

enum {
	REG_EAX = 1,
	REG_ECX = 2,
	REG_EDX = 3,
	REG_EBX = 4,
	REG_ESP = 5,
	REG_EBP = 6,
	REG_ESI = 7,	
	REG_EDI = 8,
	REG_EIP = 9,
	REG_AX = 10,
	REG_CX = 11,
	REG_DX = 12,
	REG_BX = 13,
	REG_BP = 14,
	REG_SI = 15,
	REG_DI = 16,
	REG_AH = 17,
	REG_CH = 18,
	REG_DH = 19,
	REG_BH = 20,
	REG_AL = 21,
	REG_CL = 22,
	REG_DL = 23,
	REG_BL = 24
};

union EAX {
	unsigned int u;
	int s;
};
union ECX {
	unsigned int u;
	int s;
};
union EDX {
	unsigned int u;
	int s;
};
union EBX {
	unsigned int u;
	int s;
};
union ESP {
	unsigned int u;
	int s;
};
union EBP {
	unsigned int u;
	int s;
} ;
union ESI {
	unsigned int u;
	int s;
};
union EDI {
	unsigned int u;
	int s;
};

union Template {
	unsigned int u;
	int s;
} Template;

typedef struct _VMREGS {
	union EAX EAX;
	union ECX ECX;
	union EDX EDX;
	union EBX EBX;
	union ESP ESP;
	union EBP EBP;
	union ESI ESI;
	union EDI EDI;
	
	unsigned int EIP;


} VMRegs;

struct _Instruction {

	int type;
	union {
		unsigned int u;
		signed int s;
	} imm;
	bool isRegisterDest;
	bool isPointerDest;
	bool isRegisterSrc;
	bool isPointerSrc;
	void *regDest;
	void *regSrc;
	void (*handler)();
	int regmodSrc;
	int regmodDest;
	bool isImm;
	bool sign;
};

typedef struct _VMContext {
	struct _VMREGS regs;
	struct _Instruction *instruction;
	int nInstructions;
} VM;

#endif