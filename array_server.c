#include "add.h"

int *
add_1_svc(numbers *argp, struct svc_req *rqstp)
{
	static int result=0;
	printf("Got a request of size %d\n", argp->size);
	int i;
	for(i=0; i<argp->size; i++) {
		result += argp->nums[i];
	}
	return &result;
}
