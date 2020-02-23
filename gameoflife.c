#include<stdio.h>
#include<stdlib.h>
#include"gol.h"


int main(int argc, char *argv[]) {

	// State standard rules
	int generations = 5;
	int rule = 0; // 0 = Standard & 1 = Torus
	int print_stats = 0; // 0 = NO & 1 = YES
	int input_check = 0;
	int output_check = 0;

	FILE *infile;
	FILE *outfile;

	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {

			switch(argv[i][1]) {

				default:
					fprintf(stderr, "Unknown option %s\n", argv[i]);
					exit(1);

				case 'i':
					input_check = 1;
					infile = fopen(argv[i+1], "r");
					if(infile == NULL) {
						fprintf(stderr, "Can't open file/file does not exist\n");
						exit(1);
					}
					break;

				case 'o':
					output_check = 1;
					outfile = fopen(argv[i+1], "w");
          if(outfile == NULL) {
            fprintf(stderr, "File cannot be created\n");
            exit(1);
          }
					break;

				case 'g':
					generations = atoi(argv[i+1]);
					break;

				case 's':
					print_stats = 1;
					break;

				case 't':
					rule = 1;
					break;
			}
		}
	}

	struct universe v;
	int(*rule_pick)(struct universe *u, int column, int row);

	// Determine whether to use standard or torus
	if(rule == 0) {
		rule_pick = &will_be_alive;
	}
	else {
		rule_pick = &will_be_alive_torus;
	}
	// Determine whether to read in from file or shell
	if(input_check == 0) {

		read_in_file(stdin, &v);
	}
	else {
		read_in_file(infile, &v);
	}
	// Evolve for generations times
	for(int j = 0; j < generations; j++) {
		evolve(&v, rule_pick);
	}
	// Print stats if stated
	if(print_stats == 1) {
		print_statistics(&v);
	}
	// Determine whether to write out to file or shell
	if(output_check == 0) {
		write_out_file(stdout, &v);
	}
	else {
		write_out_file(outfile, &v);
	}

	infile = NULL;
	outfile = NULL;

	return 0;
}
