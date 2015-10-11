#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* fp;
	
	if((fp = fopen("MYFIFO", "w")) == NULL)
		perror("Error");
	
	printf("Name is being Written\n");
	fputs("Utkarsh Trivedi", fp);

	fclose(fp);
	return 0;
}
