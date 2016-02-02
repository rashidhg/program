#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int i;
	char buf[30];
	for(i=10001;i<=18086;i++)
	{
		sprintf(buf,"%s%d","s_to_c",i);
		unlink(buf);
		sprintf(buf,"%s%d","c_to_s",i);
		unlink(buf);
	}
return 0;
}
