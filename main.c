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
				return 0;
				break;
			default:
				printf("Invalid argument(%s), skip it.\n", optarg);
		}
	}
}

int check_param(void)
{
	if (!g_sz_unit || !g_sz_alloc)
		return -1;

	return 0;
}

void help(void)
{
	printf("Usage :\n");
	printf(" -s	specify size of allocation and unit\n");
	printf("	i.e) request 10MB allocation\n");
	printf("	-s 10M\n");
	printf("	i.e) request 1MB allocation\n");
	printf("	-s 1M\n");
}

int main(int argc, char** argv)
{
	int i;
	int ret;

	parse_opt(argc, argv);

	ret = check_param();

	if (ret < 0){
		help();
		return -1;
	}

	return 0;
}
