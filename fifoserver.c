#include <stdio.h>
#include <linux/stat.h>
#include <sys/stat.h>

int main()
{
	FILE* fp;
	char buf[100];

	umask(0);
	mknod("MYFIFO", S_IFIFO | 0666, 0);
	
	while(1)
	{
		fp = fopen("MYFIFO", "r");
		fgets(buf, 100, fp);
		printf("Received : %s", buf);
		fclose(fp);
	}

	return 0;
}
