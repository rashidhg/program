#include "square.h"

int *
square_1_svc(args *argp, struct svc_req *rqstp)
{
	static int  result;
	printf("Got a request Number %d\n", argp->num);
	result=argp->num*argp->num;
	return &result;
}