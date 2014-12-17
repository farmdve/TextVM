#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <strings.h>

#define FLAG_CLEAR(reg, flag) (*reg &= ~(1 << flag))
#define FLAG_SET(reg, flag) (*reg |= (1 << flag))
#define FLAG_CHECK(reg, flag) (reg & (1 << flag))
#define SIGN_BIT 31

enum {
	NOP_INSTRUCTION = 0,
	MOV_INSTRUCTION = 1,
	ADD_INSTRUCTION = 2,
	SUB_INSTRUCTION = 3,
	MUL_INSTRUCTION = 4,
	DIV_INSTRUCTION = 5,
	XOR_INSTRUCTION = 6
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

enum {
	CF = 0,
	PF = 2,
	AF = 4,
	ZF = 6,
	SF = 7,
	TF = 8,
	DF = 10,
	OF = 11
};

/*
struct EFLAGS {

	unsigned int CF:1;
	unsigned int PF:1;
	unsigned int AF:1;
	unsigned int ZF:1;
	unsigned int SF:1;
	unsigned int TF:1;
	unsigned int DF:1;
	unsigned int OF:1;
	unsigned int padding:24;
}
*/

typedef struct _VMREGS {
	unsigned int EAX;
	unsigned int ECX;
	unsigned int EDX;
	unsigned int EBX;
	unsigned int ESP;
	unsigned int EBP;
	unsigned int ESI;
	unsigned int EDI;
	unsigned int EIP;
	
	unsigned int EFLAGS;
} VMRegs;

struct _Instruction {
	int type;
	int size;
	unsigned int imm;
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
};

struct instruction_table {
	int type;
	char *ins;
	void (*opcode)();

};

struct register_table {
	int type;
	char *reg;
	int size;
	void *pReg;
	char padding[3];
};

typedef struct _VMContext {
	struct _VMREGS regs;
	struct _Instruction *instruction;
	int nInstructions;
} VM;


extern VM *vm;

#include "opcodes.h"
#include "parser.h"

#endif