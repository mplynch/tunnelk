#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

int foo(int bar, const char* filename, long offset, long length, std::string& message)
{
	const int BUFFSIZE = 128;
	static char buffer[BUFFSIZE] = { "ERROR: Throw me down." };

	if (FILE* fp = fopen(filename, "r")) {
		fseek(fp, offset, SEEK_SET);

                char* line = buffer;
		for (long nbytes=0; nbytes<=length && line!=NULL;) {
			if (line = fgets(buffer, BUFFSIZE, fp)) {
				nbytes += strlen(line);
				message += line;
			}
		}

		fclose(fp);
	}

	return bar+3;
}

/*
int main()
{
	std::string message;
	int ret = foo(42, "input.in", 0, 1408, message);
	printf("ret = '%d'\n", ret);
	printf("message = '%s'\n", message.c_str());
	return 0;
}
*/
