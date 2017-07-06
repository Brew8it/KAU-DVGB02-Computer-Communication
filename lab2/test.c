#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <sys/time.h>
#include <stdint.h>


int main(int argc, char const *argv[])
{
	int time = 0;
	struct timeval start,end;

	gettimeofday(&start, NULL);
	for (int i = 0; i < 10000; ++i)
	{
		
	}
	gettimeofday(&end, NULL);
	time += ((int)((end.tv_sec-start.tv_sec) * (int)1e6 + (end.tv_usec - start.tv_usec)));

	printf("%d\n",time);
	return 0;
}