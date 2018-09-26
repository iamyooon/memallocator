#include <stdio.h>	// printf()
#include <sys/mman.h>	// mmap()
#include <unistd.h>	// getopt()
#include <string.h>	// strlen()
#include <stdlib.h>	// abort()
#include <errno.h>	// perrno(), errno
#include <limits.h>	// LONG_MAX

const char *str_szunit[] = {
	"GB",
	"MB",
	"KB",
	"B",
};

enum szunit {
	SZ_GB,
	SZ_MB,
	SZ_KB,
	SZ_B,
	NR_SZUNIT
};

unsigned int g_sz_alloc;
enum szunit g_sz_unit;

unsigned int get_size_of_allocation(const char* str, int base)
{
	unsigned long val;
	char *endptr;

	/* To distinguish success/failure after call */
	errno = 0;
	val = strtol(str, &endptr, base);

	/* Check for various possible errors */
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
		|| (errno != 0 && val == 0)) {
		perror("strtol");
		exit(EXIT_FAILURE);
	}

	if (endptr == str) {
		fprintf(stderr, "No digits were found\n");
		exit(EXIT_FAILURE);
	}

	/* If we got here, strtol() successfully parsed a number */
	return val;

}

enum szunit get_unit_of_size(const char* str)
{

	char unit;

	unit = str[strlen(str)-1];

	switch(unit){
		case 'G':
			return SZ_GB;
			break;
		case 'M':
			return SZ_MB;
			break;
		case 'K':
			return SZ_KB;
			break;
		case 'B':
			return SZ_B;
			break;
		default:
			printf("You don't give me valid size unit, use MB\n");
			return SZ_MB;
			break;
	}
}

int parse_opt(int argc, char **argv)
{
	int c;
	opterr = 0;

	/*
	 * -s size of requested allocation
	 *	G - GB, M - MB, K - KB, B - B
	 */
	while ((c = getopt(argc, argv, "s:o:")) != -1){
		switch (c)
		{
			case 's':
				g_sz_unit = get_unit_of_size(optarg);
				g_sz_alloc = get_size_of_allocation(optarg, 10);
				printf("Requested alloc size -> %d(%s)\n",g_sz_alloc, str_szunit[g_sz_unit]);
				break;
			default:
				printf("Invalid argument(%s), skip it.\n", optarg);
		}
	}
}

int main(int argc, char** argv)
{
	int i;

	parse_opt(argc, argv);

	return 0;
}
