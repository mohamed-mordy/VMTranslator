#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dict.h"

struct code {
	char *vm;
	char *assem;
};
extern char file_name[];

struct code memcode[] = {
	{"push constant", "@%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push constant i
	{"push local", "@LCL\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13\nM=D+M\n@R13"
		"\nA=M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push local i
	{"pop local", "@LCL\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13\nM=D+M\n@SP"
		"\nM=M-1\n@SP\nA=M\nD=M\n@R13\nA=M\nM=D\n"}, // pop local i
	{"push argument", "@ARG\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13"
		"\nM=D+M\n@R13\nA=M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push argument i
	{"pop argument", "@ARG\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13"
		"\nM=D+M\n@SP\nM=M-1\n@SP\nA=M\nD=M\n@R13\nA=M\nM=D\n"}, // pop argument i
	{"push this", "@THIS\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13"
		"\nM=D+M\n@R13\nA=M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push this i
	{"pop this", "@THIS\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13"
		"\nM=D+M\n@SP\nM=M-1\n@SP\nA=M\nD=M\n@R13\nA=M\nM=D\n"}, // pop this i
	{"push that", "@THAT\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13"
		"\nM=D+M\n@R13\nA=M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push that i
	{"pop that", "@THAT\nD=M\n@R13\nM=D\n@%d\nD=A\n@R13"
		"\nM=D+M\n@SP\nM=M-1\n@SP\nA=M\nD=M\n@R13\nA=M\nM=D\n"}, // pop that i
	{"push temp", "@5\nD=A\n@R13\nM=D\n@%d\nD=A\n@R13\nM=D+M\n@R13"
		"\nA=M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push temp i
	{"pop temp", "@5\nD=A\n@R13\nM=D\n@%d\nD=A\n@R13\nM=D+M\n"
		"@SP\nM=M-1\n@SP\nA=M\nD=M\n@R13\nA=M\nM=D\n"}, // pop temp i
	{"push pointer", "@%s\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push pointer 0/1
	{"pop pointer", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@%s\nM=D\n"}, // pop pointer 0/1
	{"pop static", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@%s.%d\nM=D\n"}, // pop static i
	{"push static","@%s.%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // push static i
};

struct code arithcode[] = {
	{"add", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@R13\nM=D\n@SP\nM=M-1\n@SP\nA=M\nD=M"
		"\n@R13\nM=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // add
	{"sub", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@R13\nM=D\n@SP\nM=M-1\n@SP\nA=M\nD=M"
		"\n@R13\nM=D-M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"}, // sub
	{"neg", "@SP\nM=M-1\n@SP\nA=M\nM=-M\n@SP\nM=M+1\n"}, // neg

	{"eq", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@SP\nM=M-1\n@SP\nA=M\nD=M-D\n@IF%d\nD;JEQ\n@0\nD=A\n@SP\nA=M"
		"\nM=D\n@SP\nM=M+1\n@EIF%d\n0;JMP\n(IF%d)\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n(EIF%d)\n"}, // eq
	{"gt", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@SP\nM=M-1\n@SP\nA=M\nD=M-D\n@IF%d\nD;JGT\n@0\nD=A\n@SP\nA=M\nM=D\n"
		"@SP\nM=M+1\n@EIF%d\n0;JMP\n(IF%d)\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n(EIF%d)\n"}, // gt
	{"lt", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@SP\nM=M-1\n@SP\nA=M\nD=M-D\n@IF%d\nD;JLT\n@0\nD=A\n@SP\nA=M\nM=D\n"
		"@SP\nM=M+1\n@EIF%d\n0;JMP\n(IF%d)\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n(EIF%d)\n"}, // lt

	{"and", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@SP\nM=M-1\n@SP\nA=M\nM=D&M\n@SP\nM=M+1\n"}, // and
	{"or", "@SP\nM=M-1\n@SP\nA=M\nD=M\n@SP\nM=M-1\n@SP\nA=M\nM=D|M\n@SP\nM=M+1\n"}, // or
	{"not", "@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n"}, // not
};

int memlen = sizeof(memcode) / sizeof(memcode[0]);
int arithlen = sizeof(arithcode) / sizeof(arithcode[0]);

static int get_index(char *str, int len, int flag)
{
	int i;
	if (flag)
	{
		for (i = 0; i < len; i++)
			if (!strcmp(arithcode[i].vm, str)) {
				return i;
			}
	} else {
		for (i = 0; i < len; i++)
			if (!strcmp(memcode[i].vm, str)) {
				return i;
			}
	}
	fprintf(stderr, " error, from get_index function\n");
	exit(1);
}

void get_arith_code(char *vm_inst, char *asm_code)
{
	static int i = 0;
	if (!strcmp(vm_inst, "eq") || !strcmp(vm_inst, "gt") || !strcmp(vm_inst, "lt")) {
		sprintf(asm_code, arithcode[get_index(vm_inst, arithlen, 1)].assem, i, i, i, i);
		i++;
	} else {
		strcpy(asm_code, arithcode[get_index(vm_inst, arithlen, 1)].assem);
	}
}

void get_mem_code(char *str, int val, char *asm_code)
{
	if (!strcmp(str, "pop static")) {
		sprintf(asm_code, memcode[get_index(str, memlen, 0)].assem, file_name, val);
	} else if (!strcmp(str, "push static")) {
		sprintf(asm_code, memcode[get_index(str, memlen, 0)].assem, file_name, val);
	} else if (!strcmp(str, "push pointer")) {
		sprintf(asm_code, memcode[get_index(str, memlen, 0)].assem, val ? "THAT" : "THIS");
	} else if (!strcmp(str, "pop pointer")) {
		sprintf(asm_code, memcode[get_index(str, memlen, 0)].assem, val ? "THAT" : "THIS");
	} else {
		sprintf(asm_code, memcode[get_index(str, memlen, 0)].assem, val);
	}
}

