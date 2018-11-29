#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

bool alpha_flag = false,
	 byte_flag = false,
	 nibble_flag = false;
	
void
usage(void)
{
	printf("dtob prints the binary form as stored in memory of the given input.\n\n");
	printf("Usage:\n");
	printf("dtob <num> (dtob 123)\n");
	printf("dtob <float> (dtob 123.45)\n");
	printf("dtob <string> (dtob hello)\n");
	printf("\n");
	printf("Flags:\n");
	printf("-b: insert space between each byte\n");
	printf("-n: insert space between each nibble\n");
	printf("-s: force dtob to interpret input as string\n");
}

static int
dtob(char *num, size_t size)
{
	int i;
	bool binary;
	
	for ( ; size; size--, num++) {
		for (i = 7; i > -1; i--) {
			binary = *num & (1 << i);
			printf("%d", binary);
			
			if (i == 4 && nibble_flag)
				printf(" ");
		}
		
		if (byte_flag)
			printf(" ");
	}
	
	return 0;
}

static int
print_alpha(char *value)
{
	void *ptr;
	
	ptr = value;
	printf("Binary represntation of string \"%s\":\n", value);
	dtob(ptr, strlen(value));
	
	return 0;
}

/*
#define PRINT_NUM(type, var) \
	type var; \
	void *ptr; \
	\
	var = ato ## var(value); \
	ptr = &var; \
	printf("Binary represntation of " type " \"%f\":\n", var); \
	dtob(ptr, sizeof(var)); \
	\
	return 0;
*/

static int
print_float(char *value)
{
	// PRINT_NUM(float, f);
	float f;
	void *ptr;
	
	f = atof(value);
	ptr = &f;
	printf("Binary represntation of float \"%f\":\n", f);
	dtob(ptr, sizeof(f));
	
	return 0;
}

static int
print_int(char *value)
{
	int i;
	void *ptr;
	
	i = atoi(value);
	ptr = &i;
	printf("Binary represntation of integer \"%d\":\n", i);
	dtob(ptr, sizeof(i));
	
	return 0;
}

static int
determine_type(char *value)
{
	char *char_ptr;
	
	for (char_ptr = value; *char_ptr; char_ptr++) {
		if (isalpha(*char_ptr) || alpha_flag) {
			return print_alpha(value);
		} else if (*char_ptr == '.') {
			return print_float(value);
		} else if (!*(char_ptr + 1))
			return print_int(value);
	}
	
	return -1;
}

int
main(int argc, char *argv[])
{
	char opt;
	
	while ((opt = getopt(argc, argv, "bns")) != -1) {
		switch (opt) {
			case 's':
				alpha_flag = true;
				break;
			case 'n':
				nibble_flag = true;	/* not a bug */
			case 'b':
				byte_flag = true;
				break;
			default:
				usage();
				exit(1);
		}
	}
	
	if (determine_type(argv[argc - 1]))
		printf("Could not determine input type.");
	printf("\n");
	
	return 0;
}
