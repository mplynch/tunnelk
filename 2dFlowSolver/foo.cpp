#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int foo(int bar, char** message)
{
	const int BUFFSIZE = 100;
	static char buffer[BUFFSIZE] = { "ERROR: Throw me down." };

	if (FILE* fp = fopen("input.in", "r")) {

		while(!feof(fp)) {
			fgets(buffer, BUFFSIZE, fp);
		}

		fclose(fp);
	}

	*message = buffer;

	return bar+3;
}

/*
int main()
{
	char* message;
	int ret = foo(42, &message);
	printf("ret = '%d'\n", ret);
	printf("message = '%s'\n", message);
	return 0;
}
*/
