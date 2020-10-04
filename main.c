#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

char file_name[32];

int main(int argc, char **argv)
{
	FILE *infile, *outfile;
	char buf[50];
	char line[1000];

	if (argc != 2) {
		fprintf(stderr, "Usage: VMTranslator fileName.vm\n");
		exit(1);
	}
	check_file_name(argv[1]); // make sure it is a .vm file
	make_file_name(argv[1]); // store the name of the input file without the extension in file_name[]
	sprintf(buf, "%s.asm", file_name);
	infile = fopen(argv[1], "r");
	if (infile == NULL) {
		fprintf(stderr, "%s: con't open (%s)\n", argv[0], argv[1]);
		exit(1);
	}
	outfile = fopen(buf, "w");
	if (outfile == NULL) {
		fprintf(stderr, "%s: can't open (%s)\n", argv[0], buf);
		exit(1);
	}
	while (mygetline(infile, line)) {
		if (iscomment(line))
			continue;
		code_writer(outfile, line);
	}
	fclose(infile);
	fclose(outfile);
	fprintf(stderr, "done successfully\ncheck output\n");
	return 0;
}

