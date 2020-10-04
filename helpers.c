#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dict.h"
#include "mystring.h"
#include "helpers.h"

extern char file_name[];

// static functions
static void cleanup(char *vm_line)
{
	char *vm_inst = vm_line; 

	while (*vm_line == ' ')
		vm_line++;
	while (*vm_line && *vm_line != '/' && *(vm_line + 1) != '/') {
		if (*vm_line == ' ' && *(vm_line - 1) == ' ') {
			vm_line++;
			continue;
		}
		*vm_inst++ = *vm_line++;
	}
	if (*(vm_inst - 1) == '\n' || *(vm_inst - 1) == ' ')
		*(vm_inst - 1) = '\0';
	*vm_inst = '\0';
}

static void parseline(char *vm_inst, char *str, int *val)
{
	char buf[10];
	char *ptr = buf;
	while (*vm_inst && (isalpha(*vm_inst) || isspace(*vm_inst)))
		*str++ = *vm_inst++;
	str--;
	*str = '\0';
	while (*ptr++ = *vm_inst++)
		;
	*val = atoi(buf);
}

static int count_space(char *ptr)
{
	int i = 0;
	while (*ptr) {
		if (isspace(*ptr))
			i++;
		ptr++;	
	}
	return i;
}

// functions
void check_file_name(const char *file_name)
{
	int n;
	char buf[5];

	n = strlen(file_name);
	mysubstring(buf, file_name, n- 3, n - 1);
	if (strcmp(buf, ".vm")) {
		fprintf(stderr, "provide a valid .vm file\n");
		exit(1);
	}
}

void make_file_name(const char *str)
{
	int n;

	n = strlen(str);
	mysubstring(file_name, str, 0, n - 4);
}

int iscomment(char *line)
{
	if (*line == '\n')
		return 1;
	while (*line == ' ')
		line++;
	if (*line == '/' && *(line + 1) == '/')
		return 1;
	return 0;
}

int mygetline(FILE *file, char *buf)
{
	int c;
	char *s = buf;

	while ((c = fgetc(file)) != EOF && c != '\n')
		*buf++ = c;
	if (c == '\n')
		*buf++ = '\n';
	*buf = '\0';
	return (int) (buf - s);
}

void code_writer(FILE *file, char *vm_inst)
{
	char asm_code[1000];
	char str[50];
	int val;
	fprintf(file, "// %s", vm_inst);
	cleanup(vm_inst);
	int cspace = count_space(vm_inst);
	if (cspace == 0) {
		get_arith_code(vm_inst, asm_code);
		fprintf(file, "%s", asm_code);
	} else if (cspace == 2) {
		parseline(vm_inst, str, &val);
		get_mem_code(str, val, asm_code);
		fprintf(file, "%s", asm_code);
	}
}
