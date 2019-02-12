
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

/* Global variables */

char *bin_name;
int num_workers;
char *input_file = "input.dat";
char *output_file = "output.dat";
FILE *input;
FILE *output;

/* Common functions */

void error() {
	char buf[1024];
	strcpy(buf, bin_name);
	strcat(buf, ": Error");
	perror(buf);
	exit(1);
}

/* Worker process */

void worker_proc() {
	exit(0);
}

/* Main process */

void print_pids(pid_t *children, pid_t parent) {
	FILE *output = fopen(output_file, "a");
	fprintf(output, "All children were:");
	for (int i = 0; i < num_workers; i++) {
		fprintf(output, " %d", children[i]);
	}
	fprintf(output, "\nParent was: %d\n", parent);
}

void main_proc() {
	input = fopen(input_file, "r");
	if (input == NULL)
		error();
	output = fopen(output_file, "w");
	if (output == NULL)
		error();
	fclose(output);

	num_workers = 0;
	fscanf(input, "%d", &num_workers);
	pid_t *pids = malloc(sizeof(pid_t) * num_workers);
	if (pids == NULL)
		error();
	pid_t my_pid = getpid();

	for (int i = 0; i < num_workers; i++) {
		int pid = fork();
		if (pid == -1)
			error();
		if (pid == 0)
			worker_proc();
		pids[i] = pid;
		if (wait(NULL) <= 0)
			error();
	}
	print_pids(pids, my_pid);
}

/* Pre-fork functions */

void show_usage() {
	printf("Usage: %s [options]\n", bin_name);
	printf("Options:\n");
	printf("	-h		Show this message\n");
	printf("	-i INPUT	Set input filename. Default: input.dat\n");
	printf("	-o OUTPUT	Set output filename. Default: output.dat\n");
}

void do_args(int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "hi:o:")) != -1) {
		switch(opt) {
			case 'h':
				show_usage();
				exit(0);
			case 'i':
				input_file = optarg;
			case 'o':
				output_file = optarg;
		}
	}
}

int main(int argc, char **argv) {
	bin_name = argv[0];
	do_args(argc, argv);
	main_proc();
}
