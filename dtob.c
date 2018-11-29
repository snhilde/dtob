#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

bool byte_break = false, nibble_break = false;
	
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
			
			if (i == 4 && nibble_break)
				printf(" ");
		}
		
		if (byte_break)
			printf(" ");
	}
	
	return 0;
}

int
main(int argc, char *argv[])
{
	char opt, *value, *is_ptr;
	void *num_ptr;
	bool alpha_flag = false, float_flag = false;
	
	while ((opt = getopt(argc, argv, "bns")) != -1) {
		switch (opt) {
			case 's':
				alpha_flag = true;
				break;
			case 'n':
				nibble_break = true;	/* not a bug */
			case 'b':
				byte_break = true;
				break;
			default:
				usage();
				exit(1);
		}
	}
	
	value = argv[argc - 1];
	for (is_ptr = value; *is_ptr; is_ptr++) {
		if (isalpha(*is_ptr) || alpha_flag) {
			printf("Binary represntation of string \"%s\":\n", value);
			dtob(value, strlen(value));
			alpha_flag = true;
			break;
		} else if (*is_ptr == '.')
			float_flag = true;
	}
	
	if (float_flag) {
		float num = atof(value);
		num_ptr = &num;
		printf("Binary represntation of float \"%f\":\n", num);
		dtob(num_ptr, sizeof(num));
	} else if (!alpha_flag) {
		int num = atoi(value);
		num_ptr = &num;
		printf("Binary represntation of integer \"%d\":\n", num);
		dtob(num_ptr, sizeof(num));
	}
	
	printf("\n");
	
	return 0;
}
