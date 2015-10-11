#include <stdio.h>

int main()
{
	FILE *pipe_in, *pipe_out;
	char buf[100];

	pipe_in = popen("ls", "r");
	pipe_out = popen("sort", "w");

	while(fgets(buf, 100, pipe_in))
		fputs(buf, pipe_out);

	pclose(pipe_in);
	pclose(pipe_out);
	return 0;
}
