#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	DIR* directory;
	struct dirent* dir_store;
	directory = opendir(argv[1]);

	while((dir_store = readdir(directory)) != NULL)
	{
		printf("%s\n", dir_store->d_name);
	}
}
