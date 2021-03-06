#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "files.h"

void usage()
{
	// printf("Usage: -d\t: daemonize\n");
	// printf("       -k\t: stop daemon\n");
	printf("Usage: ucwiki [OPTIONS]\n");
	printf("       -i\t: input directory\n");
	printf("       -o\t: output directory\n");
}


static int check_parameters(char *input, char*output)
{
	if (input == NULL || output == NULL)
		return -1;

	if (path_reformat(input) < 0)
		goto err_input;

	if (!is_dir(input))
		goto err_input;

	if (path_reformat(output) < 0)
		goto err_output;

	if (!is_dir(output)) {
		if (create_dir(output) < 0)
			goto err_output;
	}

	return 0;

err_input:
	printf("Error : Invalid input directory\n");
	return -1;

err_output:
	printf("Error : Couldn't create output directory\n");
	return -1;
}

int main(int argc, char * argv[])
{
	int ret = 0;
	// int daemon = 0;
	int c;
	extern char *optarg;

	struct s_tree_elt tree[200];

	char input[PATH_MAX];
	char output[PATH_MAX];

	strcpy(input, INPUT_PATH);
	strcpy(output, OUTPUT_PATH);

	while ((c = getopt(argc, argv, "dki:o:h")) != -1) {
		switch(c) {
			// case 'd':
			// 	daemon = 1;
			// 	break;
			// case 'k':
			// 	// TODO
			// 	break;
			case 'i':
				if (!is_dir(optarg)) {
					printf("Error : input directory doesn't exist\n");
					return -1;
				}

				strcpy(input, optarg);
				break;
			case 'o':
				strcpy(output, optarg);
				break;
			case 'h':
				usage();
				return -1;
				break;
			default:
				usage();
				return -1;
				break;
		}
	}

	if ((ret = check_parameters(input, output)) < 0)
		goto err;

	if ((ret = process_tree(tree, input)) < 0)
		goto err;

	if ((ret = process_files(tree, output)) < 0)
		goto err;

	if (cp_dir("../extra", output) < 0)
		printf("Couldn't copy extra dir\n");

	return ret;
err:
	fprintf(stderr, "Error in wiki generation.\n");
	return ret;
}
