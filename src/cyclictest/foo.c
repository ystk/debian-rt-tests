#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define ALLOCSZ 256

int mkcmdlinestr(int argc, char **argv)
{
	int i;
	int bufsize = ALLOCSZ;
	int elementsz;
	char *str;

	if ((str = malloc(ALLOCSZ)) == NULL) {
		perror("malloc");
		exit(-1);
	}
	str[0] = '\0';

	for (i=0; i < argc; i++) {
		elementsz = strlen(argv[i]) + 2;
		if ((strlen(str) + elementsz) >= bufsize) {
			char *ptr;
			int sz = ALLOCSZ;
			while (sz < elementsz) sz *= 2;
			ptr = realloc(str, bufsize+sz);
			if (ptr == NULL) {
				perror("realloc");
				exit(-2);
			}
			str = ptr;
			bufsize += sz;
		}
		strcat(strcat(str, argv[i]), " ");
	}
	str[strlen(str) - 1] = '\0';
	return str;
}
