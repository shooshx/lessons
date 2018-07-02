#include <stdio.h>
#include <stdlib.h>

int main() {
	size_t sz, readsz;
	char* buf = NULL;

	printf("Hello World!\n");

	FILE* f = fopen("../test_file.txt", "rb");
	if (f == NULL) {
		printf("failed to open file\n");
		return 1;
	}
	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	buf = malloc(sz + 1);
	if (buf == NULL) {
		printf("failed malloc\n");
		goto done;
	}
	fseek(f, 0, SEEK_SET);
	readsz = fread(buf, 1, sz, f);
	if (readsz != sz) {
		printf("failed fread\n");
		goto done;
	}
	buf[sz] = 0;
	printf("file: %s\n", buf);

done:
	free(buf);
	fclose(f);
	return 0;
}